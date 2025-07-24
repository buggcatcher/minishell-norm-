#
#
#       needs   fixing
#
#
run_group_test "Basic Commands" "${BASIC_TESTS[@]}"
    echo -e "${GREEN}Press Enter to continue...${NC}"; read -r
    
    run_group_test "Export Tests" "${EXPORT_TESTS[@]}"  
    echo -e "${GREEN}Press Enter to continue...${NC}"; read -r
    
    run_group_test "Setup Sequence" "${SETUP_TESTS[@]}"#!/bin/bash

# Script per testare minishell con confronto side-by-side
# Usage: ./test_minishell.sh [path_to_minishell]

# Colori per output
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Configurazione
MINISHELL_PATH="${1:-./minishell}"
TEST_DIR="minishell_test_$(date +%s)"
RESULTS_DIR="test_results"
TEMP_DIR="/tmp/minishell_test"

# File temporanei
MINISHELL_OUTPUT="$TEMP_DIR/minishell.out"
BASH_OUTPUT="$TEMP_DIR/bash.out"
MINISHELL_ERR="$TEMP_DIR/minishell.err"
BASH_ERR="$TEMP_DIR/bash.err"
VALGRIND_LOG="$TEMP_DIR/valgrind.log"

# Cleanup function
cleanup() {
    echo -e "\n${YELLOW}Cleaning up...${NC}"
    cd "$ORIGINAL_PWD" 2>/dev/null
    rm -rf "$TEST_DIR" "$TEMP_DIR" 2>/dev/null
    exit
}

# Setup
setup() {
    echo -e "${BLUE}=== MINISHELL TESTER ===${NC}"
    echo -e "${CYAN}Setting up test environment...${NC}"
    
    # Salva la directory originale
    ORIGINAL_PWD=$(pwd)
    
    # Crea directory temporanee
    mkdir -p "$TEMP_DIR" 
    mkdir -p "$ORIGINAL_PWD/$RESULTS_DIR"  # Crea nella directory originale
    mkdir -p "$TEST_DIR"
    cd "$TEST_DIR" || {
        echo -e "${RED}Error: Cannot cd to test directory $TEST_DIR${NC}"
        exit 1
    }
    
    # Verifica che minishell esista
    if [ ! -f "$ORIGINAL_PWD/$MINISHELL_PATH" ]; then
        echo -e "${RED}Error: Minishell not found at $MINISHELL_PATH${NC}"
        exit 1
    fi
    
    # Verifica file di suppression
    if [ -f "$ORIGINAL_PWD/readline.supp" ]; then
        echo -e "${GREEN}✓ Found readline.supp - readline/tinfo leaks will be suppressed${NC}"
    else
        echo -e "${YELLOW}⚠ readline.supp not found - readline/tinfo leaks will be shown${NC}"
        echo -e "${YELLOW}  This may cause false positives in memory leak detection${NC}"
    fi
    
    # Verifica valgrind
    if ! command -v valgrind >/dev/null 2>&1; then
        echo -e "${RED}Error: valgrind not installed. Install with: sudo apt-get install valgrind${NC}"
        exit 1
    else
        echo -e "${GREEN}✓ Valgrind found: $(valgrind --version | head -1)${NC}"
    fi
    
    # Trap per cleanup
    trap cleanup SIGINT SIGTERM EXIT
    
    echo -e "${GREEN}Setup completed. Test directory: $TEST_DIR${NC}"
    echo -e "${CYAN}Minishell path: $ORIGINAL_PWD/$MINISHELL_PATH${NC}\n"
}

# Funzione per eseguire comando in minishell con valgrind
run_minishell() {
    local command="$1"
    local test_name="$2"
    
    echo -e "\n${PURPLE}[MINISHELL]${NC} Testing: $command"
    
    # Assicurati di essere nella directory di test
    cd "$TEST_DIR" || return 1
    
    # Crea un file di input temporaneo
    echo "$command" > "$TEMP_DIR/input.txt"
    echo "exit" >> "$TEMP_DIR/input.txt"
    
    # Esegui con valgrind se richiesto controllo memoria
    if [[ "$3" == "valgrind" ]]; then
        # Controlla se esiste il file di suppressioni readline.supp
        local supp_file=""
        if [ -f "$ORIGINAL_PWD/readline.supp" ]; then
            supp_file="--suppressions=$ORIGINAL_PWD/readline.supp"
            echo -e "${CYAN}Using suppressions file: readline.supp${NC}"
        else
            echo -e "${YELLOW}Warning: readline.supp not found, readline leaks will be shown${NC}"
        fi
        
        cd "$TEST_DIR" # Assicurati di essere nella directory corretta
        valgrind --leak-check=full \
                 --show-leak-kinds=all \
                 --track-origins=yes \
                 --track-fds=yes \
                 $supp_file \
                 --quiet \
                 --log-file="$VALGRIND_LOG" \
                 "$ORIGINAL_PWD/$MINISHELL_PATH" < "$TEMP_DIR/input.txt" \
                 > "$MINISHELL_OUTPUT" 2> "$MINISHELL_ERR"
    else
        cd "$TEST_DIR" # Assicurati di essere nella directory corretta
        "$ORIGINAL_PWD/$MINISHELL_PATH" < "$TEMP_DIR/input.txt" \
                 > "$MINISHELL_OUTPUT" 2> "$MINISHELL_ERR"
    fi
    
    local minishell_exit_code=$?
    
    # Mostra output
    if [ -s "$MINISHELL_OUTPUT" ]; then
        echo -e "${GREEN}Output:${NC}"
        cat "$MINISHELL_OUTPUT"
    fi
    
    if [ -s "$MINISHELL_ERR" ]; then
        echo -e "${RED}Errors:${NC}"
        cat "$MINISHELL_ERR"
    fi
    
    echo -e "${YELLOW}Exit code: $minishell_exit_code${NC}"
    
    # Controlla valgrind per test singoli
    if [[ "$3" == "valgrind" ]]; then
        check_valgrind_single
    fi
    
# Funzione per test di gruppo (comandi sequenziali)
run_group_test() {
    local group_name="$1"
    shift
    local commands=("$@")
    
    echo -e "\n${BLUE}=== GROUP TEST: $group_name ===${NC}"
    
    # Test in minishell (sessione continua)
    echo -e "${PURPLE}[MINISHELL] Sequential test: $group_name${NC}"
    
    # Crea script per minishell
    {
        for cmd in "${commands[@]}"; do
            echo "$cmd"
        done
        echo "exit"
    } > "$TEMP_DIR/group_input.txt"
    
    cd "$TEST_DIR" || return 1
    if $USE_VALGRIND; then
        local supp_file=""
        if [ -f "$ORIGINAL_PWD/readline.supp" ]; then
            supp_file="--suppressions=$ORIGINAL_PWD/readline.supp"
        fi
        
        valgrind --leak-check=full \
                 --show-leak-kinds=all \
                 --track-origins=yes \
                 --track-fds=yes \
                 $supp_file \
                 --quiet \
                 --log-file="$VALGRIND_LOG" \
                 "$ORIGINAL_PWD/$MINISHELL_PATH" < "$TEMP_DIR/group_input.txt" \
                 > "$MINISHELL_OUTPUT" 2> "$MINISHELL_ERR"
    else
        "$ORIGINAL_PWD/$MINISHELL_PATH" < "$TEMP_DIR/group_input.txt" \
                 > "$MINISHELL_OUTPUT" 2> "$MINISHELL_ERR"
    fi
    
    echo -e "${GREEN}Minishell output:${NC}"
    [ -s "$MINISHELL_OUTPUT" ] && cat "$MINISHELL_OUTPUT"
    [ -s "$MINISHELL_ERR" ] && echo -e "${RED}Errors:${NC}" && cat "$MINISHELL_ERR"
    
    # Test in bash (sessione continua)
    echo -e "\n${CYAN}[BASH] Sequential test: $group_name${NC}"
    
    cd "$TEST_DIR" || return 1
    {
        for cmd in "${commands[@]}"; do
            echo "$cmd"
        done
    } | bash > "$BASH_OUTPUT" 2> "$BASH_ERR"
    
    echo -e "${GREEN}Bash output:${NC}"
    [ -s "$BASH_OUTPUT" ] && cat "$BASH_OUTPUT"
    [ -s "$BASH_ERR" ] && echo -e "${RED}Errors:${NC}" && cat "$BASH_ERR"
    
    # Valgrind report se abilitato
    if $USE_VALGRIND && [[ -s "$VALGRIND_LOG" ]]; then
        echo -e "\n${PURPLE}=== Valgrind Report for $group_name ===${NC}"
        analyze_valgrind_report
    fi
}

# Funzione separata per analisi valgrind
analyze_valgrind_report() {
        echo -e "${PURPLE}=== Valgrind Memory Report ===${NC}"
        
        # Controlla errori di memoria
        if grep -q "ERROR SUMMARY: 0 errors" "$VALGRIND_LOG"; then
            echo -e "${GREEN}✓ No memory errors detected${NC}"
        else
            echo -e "${RED}✗ Memory errors detected:${NC}"
            grep "ERROR SUMMARY" "$VALGRIND_LOG"
        fi
        
        # Analizza i leak
        local definitely_lost=$(grep "definitely lost:" "$VALGRIND_LOG" | grep -o '[0-9,]* bytes' | head -1)
        local indirectly_lost=$(grep "indirectly lost:" "$VALGRIND_LOG" | grep -o '[0-9,]* bytes' | head -1)
        local possibly_lost=$(grep "possibly lost:" "$VALGRIND_LOG" | grep -o '[0-9,]* bytes' | head -1)
        local still_reachable=$(grep "still reachable:" "$VALGRIND_LOG" | grep -o '[0-9,]* bytes' | head -1)
        
        echo -e "\n${PURPLE}Memory Leak Summary:${NC}"
        
        # Definitely lost (CRITICO - sempre errore del tuo codice)
        if [[ "$definitely_lost" != "0 bytes" && -n "$definitely_lost" ]]; then
            echo -e "${RED}✗ DEFINITELY LOST: $definitely_lost - YOUR CODE HAS MEMORY LEAKS!${NC}"
        else
            echo -e "${GREEN}✓ Definitely lost: 0 bytes${NC}"
        fi
        
        # Indirectly lost (CRITICO - causato da definitely lost)
        if [[ "$indirectly_lost" != "0 bytes" && -n "$indirectly_lost" ]]; then
            echo -e "${RED}✗ INDIRECTLY LOST: $indirectly_lost - Caused by definitely lost${NC}"
        else
            echo -e "${GREEN}✓ Indirectly lost: 0 bytes${NC}"
        fi
        
        # Possibly lost (SOSPETTO - potrebbe essere il tuo codice)
        if [[ "$possibly_lost" != "0 bytes" && -n "$possibly_lost" ]]; then
            echo -e "${YELLOW}⚠ POSSIBLY LOST: $possibly_lost - Check if it's your code${NC}"
        else
            echo -e "${GREEN}✓ Possibly lost: 0 bytes${NC}"
        fi
        
        # Still reachable (INFORMATIVO - spesso librerie esterne)
        if [[ "$still_reachable" != "0 bytes" && -n "$still_reachable" ]]; then
            echo -e "${CYAN}ℹ Still reachable: $still_reachable - Likely from libraries (suppressed)${NC}"
        else
            echo -e "${GREEN}✓ Still reachable: 0 bytes${NC}"
        fi
        
        # File Descriptors
        echo -e "\n${PURPLE}File Descriptor Check:${NC}"
        if grep -q "FILE DESCRIPTORS:" "$VALGRIND_LOG"; then
            local open_fds=$(grep "Open file descriptor" "$VALGRIND_LOG" | wc -l)
            if [ "$open_fds" -gt 3 ]; then  # stdin, stdout, stderr sono sempre aperti
                echo -e "${RED}✗ Found $((open_fds - 3)) leaked file descriptors:${NC}"
                grep -A1 "Open file descriptor" "$VALGRIND_LOG" | grep -v "std"
            else
                echo -e "${GREEN}✓ No leaked file descriptors${NC}"
            fi
        else
            echo -e "${GREEN}✓ All file descriptors properly closed${NC}"
        fi
        
        # Riepilogo finale
        echo -e "\n${PURPLE}=== FINAL VERDICT ===${NC}"
        local critical_issues=0
        
        if [[ "$definitely_lost" != "0 bytes" && -n "$definitely_lost" ]]; then
            ((critical_issues++))
        fi
        if [[ "$indirectly_lost" != "0 bytes" && -n "$indirectly_lost" ]]; then
            ((critical_issues++))
        fi
        if grep -q "ERROR SUMMARY:" "$VALGRIND_LOG" && ! grep -q "ERROR SUMMARY: 0 errors" "$VALGRIND_LOG"; then
            ((critical_issues++))
        fi
        if [ "$open_fds" -gt 3 ] 2>/dev/null; then
            ((critical_issues++))
        fi
        
        if [ "$critical_issues" -eq 0 ]; then
            echo -e "${GREEN}🎉 PASS: No critical memory issues detected!${NC}"
        else
            echo -e "${RED}❌ FAIL: Found $critical_issues critical issue(s) - FIX REQUIRED${NC}"
        fi
        
        # Salva il full report per debugging
        echo -e "\n${CYAN}Full valgrind report saved to: ${VALGRIND_LOG}${NC}"
        echo -e "${CYAN}To view full report: cat ${VALGRIND_LOG}${NC}"
}

# Controlla valgrind per test singoli
check_valgrind_single() {
    if [[ -s "$VALGRIND_LOG" ]]; then
        analyze_valgrind_report
    fi
}

# Funzione per eseguire comando in bash
run_bash() {
    local command="$1"
    
    echo -e "\n${CYAN}[BASH]${NC} Testing: $command"
    
    # Esegui in bash nella stessa directory di test
    cd "$TEST_DIR" || return 1
    bash -c "$command" > "$BASH_OUTPUT" 2> "$BASH_ERR"
    local bash_exit_code=$?
    
    # Mostra output
    if [ -s "$BASH_OUTPUT" ]; then
        echo -e "${GREEN}Output:${NC}"
        cat "$BASH_OUTPUT"
    fi
    
    if [ -s "$BASH_ERR" ]; then
        echo -e "${RED}Errors:${NC}"
        cat "$BASH_ERR"
    fi
    
    echo -e "${YELLOW}Exit code: $bash_exit_code${NC}"
}

# Funzione per confronto side-by-side
compare_outputs() {
    local command="$1"
    local test_name="$2"
    
    echo -e "\n${BLUE}=== COMPARISON: $test_name ===${NC}"
    echo -e "${BLUE}Command: $command${NC}"
    
    # Crea file temporanei per il confronto
    local minishell_full="$TEMP_DIR/minishell_full.txt"
    local bash_full="$TEMP_DIR/bash_full.txt"
    
    # Prepara output completo per minishell
    {
        echo "=== MINISHELL ==="
        echo "Command: $command"
        echo "--- Output ---"
        [ -s "$MINISHELL_OUTPUT" ] && cat "$MINISHELL_OUTPUT"
        echo "--- Errors ---"
        [ -s "$MINISHELL_ERR" ] && cat "$MINISHELL_ERR"
        echo "--- End ---"
    } > "$minishell_full"
    
    # Prepara output completo per bash
    {
        echo "=== BASH ==="
        echo "Command: $command"
        echo "--- Output ---"
        [ -s "$BASH_OUTPUT" ] && cat "$BASH_OUTPUT"
        echo "--- Errors ---"
        [ -s "$BASH_ERR" ] && cat "$BASH_ERR"
        echo "--- End ---"
    } > "$bash_full"
    
    # Confronto side-by-side usando diff con colonne
    if command -v colordiff >/dev/null 2>&1; then
        diff -y -W 120 "$minishell_full" "$bash_full" | colordiff
    else
        diff -y -W 120 "$minishell_full" "$bash_full"
    fi
    
    # Salva i risultati
    {
        echo "=== TEST: $test_name ==="
        echo "Command: $command"
        echo "Date: $(date)"
        echo ""
        diff -u "$bash_full" "$minishell_full"
        echo ""
        echo "================================"
        echo ""
    } >> "$ORIGINAL_PWD/$RESULTS_DIR/comparison_$(date +%Y%m%d_%H%M%S).log"
}

# Funzione per test con heredoc
test_heredoc() {
    local delimiter="$1"
    local content="$2"
    local test_name="$3"
    
    echo -e "\n${BLUE}=== HEREDOC TEST: $test_name ===${NC}"
    
    # Test heredoc in minishell
    echo -e "${PURPLE}[MINISHELL] Heredoc test${NC}"
    {
        cat << EOF
cat << $delimiter
$content
$delimiter
exit
EOF
    } | "$ORIGINAL_PWD/$MINISHELL_PATH" > "$MINISHELL_OUTPUT" 2> "$MINISHELL_ERR"
    
    echo "Minishell output:"
    cat "$MINISHELL_OUTPUT"
    
    # Test heredoc in bash
    echo -e "${CYAN}[BASH] Heredoc test${NC}"
    {
        cat << $delimiter
$content
$delimiter
    } > "$BASH_OUTPUT" 2> "$BASH_ERR"
    
    echo "Bash output:"
    cat "$BASH_OUTPUT"
    
    # Confronta
    echo -e "${YELLOW}Comparison:${NC}"
    diff -u "$BASH_OUTPUT" "$MINISHELL_OUTPUT" || echo "Outputs differ"
}

# Array dei test da eseguire - divisi in gruppi logici
declare -a BASIC_TESTS=(
    "> test.file"
    "echo \"hello world\" -n"
    "echo \$HOME\$"
    "echo \$\$\$USER"
    "echo \"ls | incorrect\""
    "echo hello >> file.txt"
    "echo ciao | cat -e"
    "ls -lha"
    "touch doc1.txt doc2.txt doc3.txt"
    "ls | grep \"file.txt\""
    "ls -1 | grep \".txt\" | sort > lista_documenti_ordinati.txt"
    "ls | ls | ls"
    "cat | cat | cat"
    "echo \"last exit status is \$?. last pid is \$\$\""
)

declare -a EXPORT_TESTS=(
    "export _key1=\"\""
    "export _key2="
    "export _key3=value"
    "export _key4=\"value\""
    "export _key5"
    "env | grep key"
    "export | grep key"
)

declare -a SETUP_TESTS=(
    "mkdir test"
    "export test_path=\$(pwd)/test"
    "export | grep test_path"
    "cd \$test_path"
    "echo \"user \$USER is at \$PWD with \$0\" > user.info"
    "echo \"last exit status is \$?. last pid is \$\$\" | cat -e >> user.info"
    "cat ../test.file"
)

# Funzione principale di test
run_tests() {
    echo -e "${BLUE}Starting comprehensive tests...${NC}"
    
    # Chiedi se usare valgrind
    if [[ -z "${SKIP_VALGRIND:-}" ]]; then
        echo -e "${YELLOW}Do you want to run with Valgrind? (slower but detects memory leaks)${NC}"
        echo -e "${CYAN}y = Yes (recommended), n = No (faster), s = Skip this prompt for all tests${NC}"
        read -p "Choice [y/n/s]: " valgrind_choice
        case "$valgrind_choice" in
            s|S) 
                export SKIP_VALGRIND=1
                USE_VALGRIND=false
                echo -e "${YELLOW}Valgrind disabled for all tests${NC}"
                ;;
            n|N) 
                USE_VALGRIND=false
                echo -e "${YELLOW}Running without Valgrind${NC}"
                ;;
            *) 
                USE_VALGRIND=true
                echo -e "${GREEN}Running with Valgrind${NC}"
                ;;
        esac
    else
        USE_VALGRIND=false
        echo -e "${YELLOW}Valgrind disabled by SKIP_VALGRIND environment variable${NC}"
    fi
    
    echo ""
    
    echo -e "${BLUE}Choose test mode:${NC}"
    echo -e "${CYAN}1 = Individual tests (each command separately)${NC}"
    echo -e "${CYAN}2 = Group tests (sequential commands)${NC}"
    echo -e "${CYAN}3 = Both modes${NC}"
    read -p "Choice [1/2/3]: " test_mode
    
    case "$test_mode" in
        2)
            echo -e "${GREEN}Running group tests...${NC}"
            run_group_test "Basic Commands" "${BASIC_TESTS[@]}"
            echo -e "${GREEN}Press Enter to continue...${NC}"; read -r
            
            run_group_test "Export Tests" "${EXPORT_TESTS[@]}"
            echo -e "${GREEN}Press Enter to continue...${NC}"; read -r
            
            run_group_test "Setup Sequence" "${SETUP_TESTS[@]}"
            ;;
        3)
            echo -e "${GREEN}Running both individual and group tests...${NC}"
            run_individual_tests
            echo -e "${GREEN}Now running group tests...${NC}"; read -r
            run_group_tests_all
            ;;
        *)
            echo -e "${GREEN}Running individual tests...${NC}"
            run_individual_tests
            ;;
    esac
}

# Funzione per test individuali
run_individual_tests() {
    local all_commands=("${BASIC_TESTS[@]}" "${EXPORT_TESTS[@]}" "${SETUP_TESTS[@]}")
    local test_count=1
    
    for cmd in "${all_commands[@]}"; do
        echo -e "\n${YELLOW}=== TEST $test_count/${#all_commands[@]} ===${NC}"
        echo -e "${CYAN}Command: $cmd${NC}"
        
        # Assicurati di essere nella directory di test
        cd "$TEST_DIR" || {
            echo -e "${RED}Error: Cannot access test directory${NC}"
            break
        }
        
        # Pulisci file temporanei del test precedente
        rm -f "$MINISHELL_OUTPUT" "$BASH_OUTPUT" "$MINISHELL_ERR" "$BASH_ERR" "$VALGRIND_LOG"
        
        # Esegui il comando in entrambe le shell
        if $USE_VALGRIND; then
            run_minishell "$cmd" "test_$test_count" "valgrind"
        else
            run_minishell "$cmd" "test_$test_count"
        fi
        
        run_bash "$cmd"
        compare_outputs "$cmd" "test_$test_count"
        
        echo -e "${GREEN}Press Enter to continue to next test (or 'q' to quit, 'v' to toggle valgrind)...${NC}"
        read -r user_input
        
        case "$user_input" in
            q|Q)
                echo -e "${YELLOW}Testing stopped by user${NC}"
                break
                ;;
            v|V)
                USE_VALGRIND=$(!$USE_VALGRIND)
                if $USE_VALGRIND; then
                    echo -e "${GREEN}Valgrind enabled${NC}"
                else
                    echo -e "${YELLOW}Valgrind disabled${NC}"
                fi
                ;;
        esac
        
        ((test_count++))
    done
}

# Funzione per tutti i group test
run_group_tests_all() {
    
    # Test specifici per heredoc
    echo -e "\n${BLUE}Testing Heredoc functionality...${NC}"
    
    # Heredoc test 1
    test_heredoc "EOF" "user \$USER
is using \$0
at \$PWD" "basic_heredoc"
    
    # Heredoc test 2  
    test_heredoc "x" "first line
second line" "simple_delimiter"
    
    echo -e "\n${GREEN}All tests completed!${NC}"
    echo -e "${CYAN}Results saved in: $RESULTS_DIR/${NC}"
}

# Funzione per mostrare help
show_help() {
    cat << 'HELP_EOF'
MINISHELL TESTER

Usage: ./test_minishell.sh [path_to_minishell]

This script tests your minishell implementation by:
1. Running each command in your minishell with Valgrind
2. Running the same command in bash
3. Comparing outputs side-by-side
4. Checking for memory leaks and file descriptor leaks

Features:
- Memory leak detection with Valgrind
- File descriptor tracking
- Side-by-side output comparison
- Heredoc testing
- Colored output for better readability
- Test result logging

Requirements:
- valgrind (for memory checking)
- colordiff (optional, for colored diffs)

Environment Variables:
- Set SKIP_VALGRIND=1 to skip valgrind checks (faster testing)

HELP_EOF
}

# Main execution
main() {
    case "${1:-}" in
        -h|--help|help)
            show_help
            exit 0
            ;;
        *)
            setup
            echo -e "${YELLOW}Ready to start testing. Press Enter to begin...${NC}"
            read -r
            run_tests
            ;;
    esac
}

# Run main function
main "$@"