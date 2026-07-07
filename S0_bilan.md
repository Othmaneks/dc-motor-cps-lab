# Bilan S0 — reponse indicielle et modeles K(PWM), tau(PWM)

**Objet** : premiere campagne d'identification du banc moteur DC. Reponses au creneau de tension (PWM), 7 niveaux, 20 runs, 90 ajustements exponentiels.

## 1. Methode

- Excitation : creneau PWM (2s ON / 2s OFF, 5 cycles par run), sens avant.
- Mesure : encodeur quadrature, comptage par interruption (fronts voie A), 100 Hz, CSV via liaison serie 115200 bauds.
- Modele ajuste sur chaque montee : v(t) = K(1 - e^(-t/tau)) par moindres carres (scipy curve_fit), sur la vitesse brute (non lissee).
- Validation des formes de courbes : leave-one-out (ajuster sur 6 niveaux, predire le 7e).

## 2. Resultats

### 2.1 Qualite de mesure
- Echantillonnage : 10.00 ms ± 1 ms, zero echantillon perdu sur 20 runs.
- Repetabilite intra-run : ecart-type < 1% sur K (5 cycles).
- Reproductibilite inter-jours : +0.9% sur K a PWM 150 (KPI-01 σ<5% largement tenu).

### 2.2 Modeles identifies (domaine de validite : PWM 60 a 255, a vide)

- **K(PWM) = 369.2 (1 - e^(-PWM/60.6))** [tr/min] — erreur de prediction LOO : 3.9 tr/min (~1.2%)
- **tau(PWM) = 29.3 + 383.6 e^(-PWM/86.5)** [ms] — erreur de prediction LOO : 1.2 ms (~1%)
- Loi inverse (feedforward) : **PWM = -60.6 ln(1 - K/369.2)** — verifiee sur PWM 100 (predit 101 pour 300 tr/min mesures a 300.4)

### 2.3 Constat principal : non-linearite de l'actionneur
La chaine driver+moteur est fortement non lineaire en PWM (K non proportionnel, tau non constant : 221 -> 49 ms).
Cause probable : mode drive-coast du VNH5019 (roue libre entre impulsions).
Le moteur seul est vraisemblablement quasi lineaire en tension reelle — a confirmer en S7 (mesure tension/courant).
Structure de modele retenue pour la suite : non-linearite statique + dynamique lineaire (type Hammerstein), ou linearisation autour d'un point de fonctionnement.

### 2.4 Comparaison constructeur (Pololu #4752)

| Grandeur | Pololu | Banc | Verdict |
|---|---|---|---|
| Vitesse a vide @12V | 330 tr/min | 368.7 | +12% — a elucider (tension alim ? tolerance ? ticks/tour ?) |
| Vitesse arbre moteur | 10 000 tr/min | 11 060 | +11%, coherent avec la ligne du dessus |
| tau | non publie | 49 ms (12V continu) | ordre de grandeur typique petits moteurs DC |

## 3. Limites identifiees

- Banc non bride : mouvement du support au demarrage (PWM >= 200) — biais potentiel faible sur tau uniquement.
- Hypothese 960 ticks/tour non verifiee experimentalement (test scotch a faire).
- Domaine non explore : PWM < 60 (zone morte), fonctionnement en charge, sens arriere.
- Quantification visible sur les plateaux de vitesse (double quantification ticks/temps) — artefact de mesure, sans biais sur les fits.

## 4. Suite

1. Test aveugle PWM 115 (promesses : K = 314 tr/min, tau = 131 ms).
2. Verification ticks/tour + mesure VIN en charge (elucider le +12%).
3. Brider le banc, re-tester un PWM 230 (quantifier le biais tau).
4. Mesure de J par ajout d'inertie calculable sur le hub (protocole pret, en pause).
5. S7 : identification en tension/courant, modele physique complet (J, B, Kt, Ke, R, L).
