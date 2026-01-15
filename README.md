# EvoBot - Robot Éducatif Open-Hardware

**Conception et Réalisation d'un Robot Éducatif "EvoBot"**

![License](https://img.shields.io/badge/license-MIT-green)
![Platform](https://img.shields.io/badge/platform-Arduino-blue)
![Language](https://img.shields.io/badge/language-C%2B%2B-red)

## À Propos du Projet

EvoBot est un robot éducatif **open-source** et **DIY** conçu dans le cadre du Master Spécialisé en *Systèmes Intelligents pour l'Éducation* de l'École Normale Supérieure de Meknès.

### 🎯 Objectifs
- ✅ Conception CAO paramétrique sous **FreeCAD**
- ✅ Fabrication additive via impression **3D (FDM)**
- ✅ Intégration électronique complète (capteurs + actionneurs)
- ✅ Intelligence artificielle séquentielle basée sur une **Machine à États (FSM)**
- ✅ Alternative open-hardware au **Thymio II** (budget < 500 MAD)

---

## 📋 Équipe de Réalisation

| Rôle | Personne |
|------|----------|
| **Réalisateurs** | MOUSSAID Hicham, EL HANINE Amine |
| **Encadrant** | Pr. Ahmed REGRAGUI |
| **Institution** | École Normale Supérieure, Meknès |
| **Année** | 2025-2026 |

---

## 🤖 Spécifications Techniques

### Architecture Mécanique
| Composant | Valeur |
|-----------|--------|
| Longueur | 160 mm |
| Largeur | 130 mm |
| Hauteur | 80 mm |
| Poids | ~500 g |
| Matériau | PLA (impression 3D) |

**Pièces imprimées :**
1. **Châssis inférieur** : Structure principale en noir
2. **Battery Bridge** : Pont de batterie innovant en rouge
3. **Capot supérieur** : Interface utilisateur et protection

### Architecture Électronique

```
┌─────────────────────────────────────────────────────┐
│          Arduino Uno R3 (ATmega328P)                │
│           16 MHz, 32 Ko Flash, 5V                   │
├─────────────────────────────────────────────────────┤
│  Capteurs (Entrées)                                 │
│  • HC-SR04 : Distance (Ultrason)                    │
│  • Line Tracker 3-in-1 : Suivi de ligne (IR)       │
│                                                      │
│  Actionneurs (Sorties)                              │
│  • 2x Moteurs TT à réducteur (1:48)                 │
│  • Driver L298N : Pont-H pour moteurs              │
│                                                      │
│  Alimentation                                       │
│  • 2x Batteries Li-ion 18650 en série (7.4V)       │
└─────────────────────────────────────────────────────┘
```

### Modes de Fonctionnement

**1. Mode Suivi de Ligne (Line Following)**
- 3 capteurs infrarouges pour détecter la trajectoire noire
- Algorithme PID simple pour maintenir le robot sur la ligne

**2. Mode Évitement d'Obstacles (Obstacle Avoidance)**
- Capteur ultrasonique HC-SR04
- Machine à états avec 4 transitions :
  - Avancer en ligne droite
  - Détecter → Reculer → Tourner
  - Reprendre la marche

**3. Mode Inactif (Idle)**
- Attente de commande via boutons poussoirs

---

## 🛠️ Installation et Configuration

### Prérequis
- **VS Code** avec l'extension **PlatformIO**
- **Arduino IDE** (optionnel)
- Compilateur AVR GCC (inclus dans PlatformIO)

### Clonage du Dépôt
```bash
git clone https://github.com/moussaidhicham/EvoBot-Educational-Robot.git
cd EvoBot-Educational-Robot
```

### Configuration de l'Environnement
1. Ouvrir le projet dans **VS Code**
2. L'extension PlatformIO détectera automatiquement `platformio.ini`
3. Sélectionner l'environnement : `Arduino Uno` (env:uno)

### Compilation
```bash
# Via PlatformIO CLI
pio run

# Ou via VS Code : Ctrl+Shift+P > PlatformIO: Build
```

### Téléversement (Upload)
```bash
# Via PlatformIO CLI
pio run -t upload

# Ou via VS Code : Ctrl+Shift+P > PlatformIO: Upload
```

---

## 📝 Structure du Projet

```
EvoBot-Educational-Robot/
├── src/
│   └── main.cpp              # Code principal (281 lignes)
├── include/                  # En-têtes personnalisés
├── lib/                      # Bibliothèques locales
├── test/                     # Tests unitaires
├── platformio.ini            # Configuration PlatformIO
├── .gitignore               # Fichiers à ignorer
└── README.md                # Ce fichier
```

---

## 🔌 Schéma de Câblage

### Assignation des Broches (Arduino Uno)

#### Capteurs (Entrées)
| Capteur | Broche | Type |
|---------|--------|------|
| Bouton Mode 1 | 3 | Digital |
| Bouton Mode 2 | A0 | Analog |
| Capteur Ligne Gauche | 2 | Digital |
| Capteur Ligne Milieu | 4 | Digital |
| Capteur Ligne Droit | 10 | Digital |
| HC-SR04 Trigger | A5 | Analog |
| HC-SR04 Echo | A1 | Analog |

#### Moteurs & Driver L298N (Sorties)
| Moteur | Fonction | Broche | Type |
|--------|----------|--------|------|
| Moteur A (Gauche) | Enable (PWM) | 6 | PWM |
| | Sens 1 | 11 | Digital |
| | Sens 2 | 9 | Digital |
| Moteur B (Droit) | Enable (PWM) | 5 | PWM |
| | Sens 1 | 8 | Digital |
| | Sens 2 | 7 | Digital |

#### Alimentation
- **Batterie 18650 (7.4V)** → L298N IN
- **L298N GND** → Arduino GND
- **L298N +5V** → Arduino 5V (via diode)

---

## 💾 Code Exemple

### Démarrage Rapide
```cpp
#include <Arduino.h>

// Pins
#define ENA 6
#define IN1 11
#define IN2 9
#define SPEED 150

void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, SPEED);
}

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
}

void loop() {
  forward();
  delay(1000);
}
```

Pour le code complet avec machine à états, consultez [src/main.cpp](src/main.cpp).

---

## 🧪 Tests et Validation

### Tests Manuels
- ✅ Avance en ligne droite
- ✅ Rotation sur place (droite/gauche)
- ✅ Détection d'obstacles (HC-SR04)
- ✅ Suivi de ligne (3 capteurs IR)
- ✅ Autonomie batterie : ~2 heures

### Résultats
| Test | Résultat | Notes |
|------|----------|-------|
| Ligne droite | ✅ Réussi | Moteurs équilibrés |
| Suivi de ligne | ✅ Réussi | Vitesse : 30 cm/s |
| Évitement | ✅ Réussi | Détection : 15-60 cm |
| Batterie | ✅ 2h | Autonomie nominale |

---

## ⚠️ Limitations et Problèmes Rencontrés

1. **Limitation I/O Arduino Uno**
   - Seulement 14 broches numériques disponibles
   - Solution : Utilisation de broches analogiques (A0-A5)

2. **Butée des Capacités**
   - 32 Ko Flash insufisants pour une IA complexe
   - Solution : Machine à états simple et efficace

3. **Dérive du Compass** (si intégré)
   - Les moteurs créent des champs magnétiques parasites
   - Solution : Placer les capteurs magnétiques loin des moteurs

4. **Variation des Capteurs IR**
   - Sensibilité à l'éclairage ambiant
   - Solution : Recalibrage du potentiomètre par utilisateur

---

## 🚀 Perspectives d'Amélioration

### Court terme
- [ ] Intégration d'un capteur de température
- [ ] Ajout de servomoteurs pour bras articulé
- [ ] Implémentation PID améliorée pour suivi de ligne

### Moyen terme
- [ ] Migration vers Arduino Mega (plus d'I/O)
- [ ] Ajout de communication Bluetooth
- [ ] Écran LCD pour feedback utilisateur

### Long terme
- [ ] Vision par caméra (OpenCV)
- [ ] Navigation GPS
- [ ] Apprentissage par renforcement (IA adaptative)

---

## 📜 Licence

Ce projet est distribué sous la licence **MIT**.

**Vous êtes libres de :**
- ✅ Utiliser le projet à titre commercial
- ✅ Modifier et distribuer le code
- ✅ Utiliser le design mécanique

**À condition de :**
- ⚠️ Inclure une copie de la licence et du copyright
- ⚠️ Documenter les changements importants

Pour plus de détails, consultez [LICENSE](LICENSE).

---

## 📚 Ressources et Références

### Documentation Officielle
- [Arduino Uno Reference](https://www.arduino.cc/reference/en/)
- [PlatformIO Docs](https://docs.platformio.org/)
- [FreeCAD CAD Guide](https://www.freecadweb.org/wiki)

### Datasheets
- [ATmega328P](https://www.microchip.com/) - Microcontrôleur
- [L298N Motor Driver](https://www.st.com/) - Pont-H
- [HC-SR04 Ultrasonic](https://www.botnroll.com/) - Capteur distance
- [18650 Li-ion Battery](https://www.samsung.com/) - Alimentation

### Bibliographie (Rapport Complet)
Consultez le document `RAPPORT_EvoBot_2025-2026.pdf` pour :
- Étude mécanique détaillée
- Schémas électroniques complets
- Algorithmes détaillés
- Résultats expérimentaux

---

## 🤝 Contribution

Les contributions sont bienvenues ! Pour toute amélioration :

1. **Fork** le dépôt
2. **Branch** (`git checkout -b feature/AmazingFeature`)
3. **Commit** vos changements (`git commit -m 'Add some AmazingFeature'`)
4. **Push** vers la branche (`git push origin feature/AmazingFeature`)
5. **Ouvrir une Pull Request**

---

## 📧 Contact et Support

- **Issues GitHub** : Pour signaler des bugs
- **Discussions** : Pour des questions générales
- **Email** : moussaidhicham@example.com

---

## 🙏 Remerciements

Un grand merci à :
- **Pr. Ahmed REGRAGUI** pour l'encadrement et les conseils
- **L'équipe pédagogique du Master SIE** de l'ENS Meknès
- **Le personnel du FabLab** pour l'accès aux machines
- **Nos camarades de promotion** pour l'esprit d'entraide

---

**Bonne exploration du monde de la robotique éducative ! 🤖**

*Projet académique - Master SIE 2025-2026*
