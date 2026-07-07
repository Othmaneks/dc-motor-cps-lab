# Campagne S0 — journal des essais

Banc : Pololu 37D 30:1 (POLOLU-4752) + VNH5019 + Arduino Uno + alim 12V GST36E12.
Protocole : creneau PWM (2s ON / 2s OFF, 5 cycles), acquisition 100 Hz, 115200 bauds.
Convention : 960 ticks/tour de sortie (fronts CHANGE sur voie A, reduction 30:1) — A VERIFIER (test scotch).

## Sessions

| Fichier | PWM | Session | Remarques |
|---|---|---|---|
| S0_step_001.csv | 150 | J1 | premier jeu de donnees, pipeline valide |
| S0_pwm150_r1..r3.csv | 150 | J2 | reference — reproduit J1 a +0.9% |
| S0_pwm080_r1..r3.csv | 80 | J2 | RAS |
| S0_pwm230_r1..r3.csv | 230 | J2 | mouvement du support au demarrage observe (banc non bride) |
| S0_pwm060_r1..r2.csv | 60 | J3 | demarrage OK, pas de blocage — zone morte < 60 |
| S0_pwm100_r1..r3.csv | 100 | J3 | RAS |
| S0_pwm200_r1..r2.csv | 200 | J3 | mouvement du support au demarrage |
| S0_pwm255_r1..r2.csv | 255 | J3 | 100% duty = 12V continu (pas de hachage) |
| S0_pwm150_hub_r1..r2.csv | 150 | J4 | controle apres montage hub universel — aucun changement (K -0.3%, tau -1.4%, dans le bruit) |

Nota : les fichiers bruts J3 etaient nommes 50/100/200/250 par erreur — niveaux reels detectes dans les donnees : 60/100/200/255.

## Resultats consolides (90 fits, 7 niveaux)

| PWM | K (tr/min) | tau (ms) |
|---|---|---|
| 60  | 230.8 ± 0.5 | 220.8 ± 2.4 |
| 80  | 271.8 ± 0.9 | 182.6 ± 2.0 |
| 100 | 300.4 ± 0.6 | 148.7 ± 1.3 |
| 150 | 336.5 ± 0.7 |  97.5 ± 1.4 |
| 200 | 352.1 ± 0.3 |  67.6 ± 1.3 |
| 230 | 359.0 ± 0.5 |  56.4 ± 2.2 |
| 255 | 368.7 ± 0.2 |  49.0 ± 2.0 |

## Questions ouvertes

1. **+12% vs Pololu** : vitesse a vide mesuree 368.7 tr/min vs 330 spec. Suspects : tension alim reelle (mesurer VIN en charge), tolerance unite (±10% courant), hypothese 960 ticks/tour (test scotch : compter tours reels vs ticks).
2. **Banc non bride** : mouvement du support au demarrage pour PWM >= 200 — biais potentiel faible sur tau (K non affecte). Brider puis re-tester un PWM 230.
3. **Test aveugle PWM 115** : le modele promet K = 314 tr/min et tau = 131 ms. A verifier au banc (1 run).
4. **Zone morte** : comportement sous PWM 60 non explore.
5. **Mesure de J** : protocole barre + masses sur hub universel — EN PAUSE (barre percee a fabriquer, pas besoin d'impression 3D).
