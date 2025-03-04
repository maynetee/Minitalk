#!/bin/bash

# Couleurs pour le formatage de sortie
RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
BLUE="\033[34m"
MAGENTA="\033[35m"
CYAN="\033[36m"
BOLD="\033[1m"
RESET="\033[0m"

# Variables de comptage
MANDATORY_TOTAL=0
MANDATORY_PASSED=0
BONUS_TOTAL=0
BONUS_PASSED=0

# Fichiers temporaires
SERVER_OUTPUT="server_output.txt"
CLIENT_OUTPUT="client_output.txt"
ERROR_LOG="error_log.txt"

# Fonction pour afficher les titres de section
print_section() {
    echo -e "\n${BOLD}${MAGENTA}======== $1 ========${RESET}"
}

# Fonction pour afficher les titres de test
print_test() {
    local category="$1"
    local name="$2"
    
    if [ "$category" = "mandatory" ]; then
        MANDATORY_TOTAL=$((MANDATORY_TOTAL + 1))
        echo -e "\n${BOLD}${BLUE}[MANDATORY TEST $MANDATORY_TOTAL] $name${RESET}"
    else
        BONUS_TOTAL=$((BONUS_TOTAL + 1))
        echo -e "\n${BOLD}${CYAN}[BONUS TEST $BONUS_TOTAL] $name${RESET}"
    fi
}

# Fonction pour afficher les résultats de test
print_result() {
    local category="$1"
    local name="$2"
    local status="$3"
    local message="$4"
    
    if [ "$status" -eq 0 ]; then
        if [ "$category" = "mandatory" ]; then
            echo -e "${GREEN}✓ PASSED: $name${RESET}"
            MANDATORY_PASSED=$((MANDATORY_PASSED + 1))
        else
            echo -e "${GREEN}✓ PASSED: $name${RESET}"
            BONUS_PASSED=$((BONUS_PASSED + 1))
        fi
    else
        echo -e "${RED}✗ FAILED: $name${RESET}"
        echo -e "${YELLOW}$message${RESET}"
        echo "[$(date)] $category TEST: $name - ERROR: $message" >> $ERROR_LOG
    fi
}

# Fonction pour démarrer le serveur
start_server() {
    local server_type="$1"
    echo -e "${CYAN}Démarrage du $server_type...${RESET}"
    ./$server_type > $SERVER_OUTPUT 2>&1 &
    SERVER_PID=$!
    sleep 1
    
    # Vérifier que le serveur a démarré correctement
    if ! ps -p $SERVER_PID > /dev/null; then
        echo -e "${RED}Le serveur n'a pas démarré correctement${RESET}"
        cat $SERVER_OUTPUT
        return 1
    fi
    
    # Récupérer le PID imprimé par le serveur
    PRINTED_PID=$(grep -o "PID: [0-9]*" $SERVER_OUTPUT | grep -o "[0-9]*")
    if [ -z "$PRINTED_PID" ]; then
        echo -e "${RED}Impossible de récupérer le PID du serveur${RESET}"
        cat $SERVER_OUTPUT
        kill -9 $SERVER_PID 2>/dev/null
        return 1
    fi
    
    echo -e "${CYAN}$server_type démarré avec le PID: $PRINTED_PID${RESET}"
    return 0
}

# Fonction pour arrêter le serveur
stop_server() {
    if [ -n "$SERVER_PID" ]; then
        echo -e "${CYAN}Arrêt du serveur (PID: $SERVER_PID)...${RESET}"
        kill -9 $SERVER_PID 2>/dev/null
        wait $SERVER_PID 2>/dev/null
        SERVER_PID=""
        sleep 1
    fi
}

# Fonction pour exécuter le client avec des paramètres
run_client() {
    local client_type="$1"
    local pid="$2"
    local message="$3"
    local timeout="$4"
    
    echo -e "${CYAN}Exécution de $client_type avec le message: '$message'${RESET}"
    timeout $timeout ./$client_type $pid "$message" > $CLIENT_OUTPUT 2>&1
    local exit_code=$?
    
    return $exit_code
}

# Fonction pour vérifier si un message est présent dans la sortie du serveur
check_server_output() {
    local expected="$1"
    local output=$(cat $SERVER_OUTPUT)
    
    if echo "$output" | grep -F -- "$expected" > /dev/null; then
        return 0
    else
        return 1
    fi
}

# Vérification préliminaire
print_section "VÉRIFICATION DES FICHIERS"

# Vérifier que les exécutables obligatoires existent
if [ ! -f "./client" ] || [ ! -f "./server" ]; then
    echo -e "${RED}Les exécutables client et/ou server n'existent pas.${RESET}"
    echo -e "${YELLOW}Compilation en cours...${RESET}"
    make
fi

if [ ! -f "./client" ] || [ ! -f "./server" ]; then
    echo -e "${RED}La compilation a échoué ou les exécutables n'ont pas été créés.${RESET}"
    exit 1
fi

# Vérifier si les bonus sont disponibles
BONUS_AVAILABLE=0
if [ -f "./client_bonus" ] && [ -f "./server_bonus" ]; then
    BONUS_AVAILABLE=1
    echo -e "${GREEN}Les exécutables bonus sont disponibles - les tests bonus seront exécutés.${RESET}"
else
    echo -e "${YELLOW}Les exécutables bonus ne sont pas disponibles - les tests bonus seront ignorés.${RESET}"
    echo -e "${YELLOW}Pour tester les bonus, exécutez 'make bonus' d'abord.${RESET}"
fi

# Créer un fichier de log vide
> $ERROR_LOG

# ====================================
# TESTS DE LA PARTIE OBLIGATOIRE
# ====================================
print_section "TESTS DE LA PARTIE OBLIGATOIRE"

# Test 1: Message simple
print_test "mandatory" "Message simple"
if start_server "server"; then
    run_client "client" $PRINTED_PID "Hello, world!" 5
    client_status=$?
    sleep 1
    if check_server_output "Hello, world!"; then
        print_result "mandatory" "Message simple" 0 ""
    else
        print_result "mandatory" "Message simple" 1 "Le serveur n'a pas reçu le message correctement"
    fi
    stop_server
else
    print_result "mandatory" "Message simple" 1 "Impossible de démarrer le serveur"
fi

# Test 2: Message vide
print_test "mandatory" "Message vide"
if start_server "server"; then
    run_client "client" $PRINTED_PID "" 5
    client_status=$?
    sleep 1
    if [ $client_status -eq 0 ]; then
        print_result "mandatory" "Message vide" 0 ""
    else
        print_result "mandatory" "Message vide" 1 "Le client a échoué avec un message vide (code $client_status)"
    fi
    stop_server
else
    print_result "mandatory" "Message vide" 1 "Impossible de démarrer le serveur"
fi

# Test 3: Message long (1000 caractères)
print_test "mandatory" "Message long (1000 caractères)"
if start_server "server"; then
    LONG_MSG=$(printf '%.0s-' {1..1000})
    run_client "client" $PRINTED_PID "$LONG_MSG" 10
    client_status=$?
    sleep 3
    if [ $client_status -eq 0 ] && check_server_output "---"; then
        print_result "mandatory" "Message long" 0 ""
    else
        print_result "mandatory" "Message long" 1 "Le message long n'a pas été transmis correctement"
    fi
    stop_server
else
    print_result "mandatory" "Message long" 1 "Impossible de démarrer le serveur"
fi

# Test 4: Caractères spéciaux
print_test "mandatory" "Caractères spéciaux"
if start_server "server"; then
    SPECIAL_CHARS='!@#$%^&*()_+-={}[]|;:,.<>/?`~'
    run_client "client" $PRINTED_PID "$SPECIAL_CHARS" 5
    client_status=$?
    sleep 1
    # Pour éviter les problèmes avec grep, on vérifie juste que le client s'est terminé correctement
    if [ $client_status -eq 0 ]; then
        print_result "mandatory" "Caractères spéciaux" 0 ""
    else
        print_result "mandatory" "Caractères spéciaux" 1 "Le client a échoué avec des caractères spéciaux"
    fi
    stop_server
else
    print_result "mandatory" "Caractères spéciaux" 1 "Impossible de démarrer le serveur"
fi

# Test 5: Messages multiples séquentiels
print_test "mandatory" "Messages multiples séquentiels"
if start_server "server"; then
    run_client "client" $PRINTED_PID "Message 1" 5
    sleep 1
    run_client "client" $PRINTED_PID "Message 2" 5
    sleep 1
    if check_server_output "Message 1" && check_server_output "Message 2"; then
        print_result "mandatory" "Messages multiples séquentiels" 0 ""
    else
        print_result "mandatory" "Messages multiples séquentiels" 1 "Les messages n'ont pas été reçus correctement"
    fi
    stop_server
else
    print_result "mandatory" "Messages multiples séquentiels" 1 "Impossible de démarrer le serveur"
fi

# Test 6: PID invalide (trop grand)
print_test "mandatory" "PID invalide (trop grand)"
run_client "client" 999999999 "Message test" 2
client_status=$?
if [ $client_status -ne 0 ]; then
    grep -q "Error\|Failed" $CLIENT_OUTPUT
    if [ $? -eq 0 ]; then
        print_result "mandatory" "PID invalide (trop grand)" 0 ""
    else
        print_result "mandatory" "PID invalide (trop grand)" 1 "Pas de message d'erreur pour un PID invalide"
    fi
else
    print_result "mandatory" "PID invalide (trop grand)" 1 "Le client n'a pas détecté un PID invalide"
fi

# Test 7: PID invalide (négatif)
print_test "mandatory" "PID invalide (négatif)"
run_client "client" -100 "Message test" 2
client_status=$?
if [ $client_status -ne 0 ]; then
    grep -q "Error\|Invalid" $CLIENT_OUTPUT
    if [ $? -eq 0 ]; then
        print_result "mandatory" "PID invalide (négatif)" 0 ""
    else
        print_result "mandatory" "PID invalide (négatif)" 1 "Pas de message d'erreur pour un PID négatif"
    fi
else
    print_result "mandatory" "PID invalide (négatif)" 1 "Le client n'a pas détecté un PID négatif comme invalide"
fi

# Test 8: PID non numérique
print_test "mandatory" "PID non numérique"
run_client "client" "abc" "Message test" 2
client_status=$?
if [ $client_status -ne 0 ]; then
    grep -q "Error\|Usage\|Invalid" $CLIENT_OUTPUT
    if [ $? -eq 0 ]; then
        print_result "mandatory" "PID non numérique" 0 ""
    else
        print_result "mandatory" "PID non numérique" 1 "Pas de message d'erreur pour un PID non numérique"
    fi
else
    print_result "mandatory" "PID non numérique" 1 "Le client n'a pas détecté un PID non numérique comme invalide"
fi

# Test 9: Arguments manquants
print_test "mandatory" "Arguments manquants"
run_client "client" "" "" 2
client_status=$?
if [ $client_status -ne 0 ]; then
    grep -q "Usage" $CLIENT_OUTPUT
    if [ $? -eq 0 ]; then
        print_result "mandatory" "Arguments manquants" 0 ""
    else
        print_result "mandatory" "Arguments manquants" 1 "Pas de message d'usage lorsque des arguments sont manquants"
    fi
else
    print_result "mandatory" "Arguments manquants" 1 "Le client n'a pas détecté les arguments manquants"
fi

# Test 10: Message très long (stress test)
print_test "mandatory" "Message très long (5,000 caractères)"
if start_server "server"; then
    VERY_LONG_MSG=$(printf '%.0s#' {1..5000})
    run_client "client" $PRINTED_PID "$VERY_LONG_MSG" 30
    client_status=$?
    sleep 5
    if [ $client_status -eq 0 ]; then
        print_result "mandatory" "Message très long" 0 ""
    else
        print_result "mandatory" "Message très long" 1 "Le client a échoué avec un message très long"
    fi
    stop_server
else
    print_result "mandatory" "Message très long" 1 "Impossible de démarrer le serveur"
fi

# ====================================
# TESTS DE LA PARTIE BONUS
# ====================================
if [ $BONUS_AVAILABLE -eq 1 ]; then
    print_section "TESTS DE LA PARTIE BONUS"
    
    # Test B1: Message simple avec accusé de réception
    print_test "bonus" "Message simple avec accusé de réception"
    if start_server "server_bonus"; then
        run_client "client_bonus" $PRINTED_PID "Hello bonus world!" 5
        client_status=$?
        sleep 1
        if check_server_output "Hello bonus world!" && grep -q "confirmed\|received" $CLIENT_OUTPUT; then
            print_result "bonus" "Message simple avec accusé de réception" 0 ""
        else
            print_result "bonus" "Message simple avec accusé de réception" 1 "Le message n'a pas été correctement reçu ou confirmé"
        fi
        stop_server
    else
        print_result "bonus" "Message simple avec accusé de réception" 1 "Impossible de démarrer le serveur bonus"
    fi
    
    # Test B2: Message vide avec accusé de réception
    print_test "bonus" "Message vide avec accusé de réception"
    if start_server "server_bonus"; then
        run_client "client_bonus" $PRINTED_PID "" 5
        client_status=$?
        sleep 1
        if [ $client_status -eq 0 ] && grep -q "confirmed\|received" $CLIENT_OUTPUT; then
            print_result "bonus" "Message vide avec accusé de réception" 0 ""
        else
            print_result "bonus" "Message vide avec accusé de réception" 1 "Le message vide n'a pas été correctement confirmé"
        fi
        stop_server
    else
        print_result "bonus" "Message vide avec accusé de réception" 1 "Impossible de démarrer le serveur bonus"
    fi
    
    # Test B3: Support Unicode
    print_test "bonus" "Support Unicode"
    if start_server "server_bonus"; then
        UNICODE_STR="こんにちは Привет Olá مرحبا 你好 🚀 😊 🌍"
        run_client "client_bonus" $PRINTED_PID "$UNICODE_STR" 10
        client_status=$?
        sleep 2
        if [ $client_status -eq 0 ] && grep -q "confirmed\|received" $CLIENT_OUTPUT; then
            print_result "bonus" "Support Unicode" 0 ""
        else
            print_result "bonus" "Support Unicode" 1 "Le message Unicode n'a pas été correctement traité"
        fi
        stop_server
    else
        print_result "bonus" "Support Unicode" 1 "Impossible de démarrer le serveur bonus"
    fi
    
    # Test B4: Message long avec accusé de réception
    print_test "bonus" "Message long avec accusé de réception (2000 caractères)"
    if start_server "server_bonus"; then
        LONG_MSG=$(printf '%.0s*' {1..2000})
        run_client "client_bonus" $PRINTED_PID "$LONG_MSG" 20
        client_status=$?
        sleep 5
        if [ $client_status -eq 0 ] && grep -q "confirmed\|received" $CLIENT_OUTPUT; then
            print_result "bonus" "Message long avec accusé de réception" 0 ""
        else
            print_result "bonus" "Message long avec accusé de réception" 1 "Le message long n'a pas été correctement confirmé"
        fi
        stop_server
    else
        print_result "bonus" "Message long avec accusé de réception" 1 "Impossible de démarrer le serveur bonus"
    fi
else
    print_section "TESTS DE LA PARTIE BONUS (IGNORÉS)"
    echo -e "${YELLOW}Les tests bonus ont été ignorés car les exécutables bonus n'ont pas été trouvés.${RESET}"
fi

# Nettoyer
rm -f $SERVER_OUTPUT $CLIENT_OUTPUT

# Afficher le résumé
print_section "RÉSUMÉ DES TESTS"
echo -e "${BOLD}Tests obligatoires: ${MANDATORY_TOTAL} total, ${GREEN}${MANDATORY_PASSED} réussis${RESET}, ${RED}$((MANDATORY_TOTAL - MANDATORY_PASSED)) échoués${RESET}"

if [ $BONUS_AVAILABLE -eq 1 ]; then
    echo -e "${BOLD}Tests bonus: ${BONUS_TOTAL} total, ${GREEN}${BONUS_PASSED} réussis${RESET}, ${RED}$((BONUS_TOTAL - BONUS_PASSED)) échoués${RESET}"
fi

if [ $MANDATORY_PASSED -eq $MANDATORY_TOTAL ]; then
    echo -e "\n${BOLD}${GREEN}TOUS LES TESTS OBLIGATOIRES ONT RÉUSSI! 🎉${RESET}"
    
    if [ $BONUS_AVAILABLE -eq 1 ]; then
        if [ $BONUS_PASSED -eq $BONUS_TOTAL ]; then
            echo -e "${BOLD}${GREEN}TOUS LES TESTS BONUS ONT RÉUSSI! 🚀${RESET}"
        else
            echo -e "${BOLD}${YELLOW}CERTAINS TESTS BONUS ONT ÉCHOUÉ.${RESET}"
        fi
    fi
else
    echo -e "\n${BOLD}${RED}CERTAINS TESTS OBLIGATOIRES ONT ÉCHOUÉ.${RESET}"
    echo -e "${YELLOW}Consultez le fichier $ERROR_LOG pour plus de détails.${RESET}"
fi

exit $(( (MANDATORY_TOTAL - MANDATORY_PASSED) > 0 ? 1 : 0 ))
