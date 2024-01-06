#include <iostream>
#include <cstdlib>
#include <cstdio>

int main() {
    std::cout << "Paketverlustüberwachung gestartet...\n";

    // Ping-Befehl ausführen und die Ausgabe überwachen
    std::string command = "ping -c 10 google.com 2>&1";  // 10 Pings an google.com senden und Fehlerausgabe abfangen
    FILE *pipe = popen(command.c_str(), "r");  // Pipe für die Ausführung des Befehls öffnen
    if (!pipe) {
        std::cerr << "Fehler beim Ausführen des Ping-Befehls.\n";
        return 1;
    }

    char buffer[128];
    std::string result = "";
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    pclose(pipe);  // Pipe schließen

    // Überprüfen, ob die Ausgabe Fehler in Bezug auf die Namensauflösung enthält
    if (result.find("Temporary failure in name resolution") != std::string::npos) {
        std::cout << "Paketverlust: Es gab ein Problem mit der DNS-Auflösung.\n";
    } else if (result.find("0% packet loss") != std::string::npos) {
        std::cout << "Paketverlust: Kein Verlust festgestellt. Das Netzwerk funktioniert korrekt.\n";
    } else {
        std::cout << "Paketverlust: Es gab einen Paketverlust. Möglicherweise liegt das Problem beim Netzwerk oder beim PC.\n";
    }

    return 0;
}
