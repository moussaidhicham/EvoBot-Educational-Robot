# EvoBot - Robot Éducatif Open-Hardware

**Conception et Réalisation d'un Robot Éducatif "EvoBot"**

![License](https://img.shields.io/badge/license-MIT-green)
![Platform](https://img.shields.io/badge/platform-Arduino-blue)
![Language](https://img.shields.io/badge/language-C%2B%2B-red)

## À Propos du Projet

EvoBot est un robot éducatif **open-source** et **DIY** conçu dans le cadre du Master Spécialisé en *Systèmes Intelligents pour l'Éducation* de l'École Normale Supérieure de Meknès.

### Objectifs
- Conception CAO paramétrique sous **FreeCAD**
- Fabrication additive via impression **3D (FDM)**
- Intégration électronique complète (capteurs + actionneurs)
- Intelligence artificielle séquentielle basée sur une **Machine à États (FSM)**
- Alternative open-hardware au **Thymio II** (budget < 500 MAD)

---

## Équipe de Réalisation

| Rôle | Personne | Dépôt GitHub |
|------|----------|--------------|
| **Réalisateur** | MOUSSAID Hicham | [moussaidhicham/EvoBot-Educational-Robot](https://github.com/moussaidhicham/EvoBot-Educational-Robot) |
| **Réalisateur** | EL HANINE Amine | [L7A9/evobot](https://github.com/L7A9/evobot) |
| **Encadrant** | Pr. Ahmed REGRAGUI | - |

---

## Spécifications Techniques

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
  - Détecter -> Reculer -> Tourner
  - Reprendre la marche

**3. Mode Inactif (Idle)**
- Attente de commande via boutons poussoirs

---

## Installation et Configuration

### Prérequis
- **VS Code** avec l'extension **PlatformIO**
- **Arduino IDE** (optionnel - nécessite de copier `src/main.cpp`)
- Compilateur AVR GCC (inclus dans PlatformIO)

### Clonage du Dépôt
```bash
# Via le dépôt de Moussaid Hicham
git clone https://github.com/moussaidhicham/EvoBot-Educational-Robot.git

# OU via le dépôt de El Hannine Amine
git clone https://github.com/L7A9/evobot.git
```

### Configuration de l'Environnement
1. Ouvrir le dossier dans **VS Code**
2. L'extension PlatformIO détectera automatiquement `platformio.ini`
3. Sélectionner l'environnement : `Arduino Uno` (env:uno)

### Compilation et Téléversement
Utilisez les icônes de la barre d'outils **PlatformIO** en bas de VS Code :
- **Compiler** : Build
- **Téléverser** : Upload
- **Moniteur Série** : Serial Monitor

---

## Alternative : Utilisation avec l'Arduino IDE

Si vous préférez ne pas utiliser VS Code, il est tout à fait possible de compiler et téléverser le code via l'**Arduino IDE** classique, sans aucune installation supplémentaire :

1.  Ouvrez l'**Arduino IDE**.
2.  Copiez l'intégralité du contenu du fichier `src/main.cpp`.
3.  Collez-le dans votre croquis (*sketch*) vide.
4.  Dans le menu **Outils**, sélectionnez la carte **Arduino Uno**.
5.  Cliquez sur le bouton **Téléverser**.

> [!TIP]
> Le code source est conçu pour être 100% autonome dans un seul fichier (`main.cpp`), ce qui garantit une compatibilité totale avec tous les éditeurs Arduino standards.

---

## Structure du Projet

```
EvoBot-Educational-Robot/
├── src/
│   └── main.cpp              # Code principal (firmware complet)
├── include/                  # En-têtes et définitions
├── platformio.ini            # Configuration PlatformIO
├── .gitignore               # Fichiers à ignorer (LaTeX, .pio, etc.)
└── README.md                # Ce fichier
```

---

## Câblage (Arduino Uno)

### Entrées (Capteurs & Boutons)
| Composant | Pin | Fonction |
|-----------|-----|----------|
| Bouton 1 | 3 | Mode Suivi de Ligne |
| Bouton 2 | A0 | Mode Évitement |
| IR Gauche | 2 | Détection Ligne |
| IR Milieu | 4 | Détection Ligne |
| IR Droit | 10 | Détection Ligne |
| Trig (US) | A5 | Déclenchement Ultrason |
| Echo (US) | A1 | Écho Ultrason |

### Sorties (Moteurs L298N)
| Fonction | Pin | Type |
|----------|-----|------|
| ENA (Gauche) | 6 | PWM |
| IN1 / IN2 | 11 / 9 | Direction Gauche |
| ENB (Droit) | 5 | PWM |
| IN3 / IN4 | 8 / 7 | Direction Droite |

---

## Résultats de Validation

| Test | Résultat | Vitesse / Précision |
|------|----------|---------------------|
| Ligne droite | Succès | Trajectoire stable |
| Suivi de ligne | Succès | ~30 cm/s |
| Évitement | Succès | Seuil : 15 cm |
| Autonomie | Succès | Batterie Li-ion 18650 |

---

## Licence

Ce projet est distribué sous la licence **MIT**. Vous êtes libres de l'utiliser, de le modifier et de le distribuer à des fins éducatives ou commerciales, sous réserve de citer les auteurs originaux.

---

## Remerciements

Nous remercions vivement **Pr. Ahmed REGRAGUI** pour son encadrement précieux et ses conseils durant ce module de robotique éducative au sein du Master SIE.

*Projet académique - ENS Meknès 2024-2025*
