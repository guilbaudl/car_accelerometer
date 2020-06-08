//broche des valeurs de l'accéléromètre
const int z = A1;
const int y = A2;
const int x = A3;

const int lednX[3] = {2,3,4}; //leds pour les x positifs
const int ledpY[3] = {5,6,7}; //leds pour les y positifs
const int ledpX[3] = {8,9,10}; //leds pour les x negatifs
const int lednY[3] = {11,12,13}; //leds pour les y negatifs

//séquences d'allumages de led pour la mise en route du matériel
const int led1[19] = {2,5,8,11,2,5,8,11,2,5,8,11,2,5,8,11,2,2,2};
const int led2[19] = {3,3,6,9,12,3,6,9,12,3,6,9,12,3,6,9,12,3,3};
const int led3[19] = {4,4,4,7,10,13,4,7,10,13,4,7,10,13,4,7,10,13,4};

//valeurs d'accélérations pour les leds
const float acc1 = 10; //seuil allumage pour la premiere led
const float acc2 = 15; //seuil allumage pour la deuxieme led
const float acc3 = 20; //seuil allumage pour la troisieme led

//grandeurs vectorielles
float vecteurInit[3]; //vecteur initial
float x0; //composante x du vecteur initial
float y0; //composante y du vecteur initial
float z0; //composante z du vecteur initial

float mesure[3] = {0}; //vecteur dont les valeurs sont changées a chaque fois

//trois vecteurs de la base de projection
float U1[3] = {0}; //vecteur vertical initial
float U2[3] = {0}; //vecteur dans le plan d'inclinaison de l'acceléromètre
float U3[3] = {0}; //troisième vecteur

//norme de chaque vecteur de la base de projection
float nU1 = 0; //norme U1
float nU2 = 0; //norme U2
float nU3 = 0; //norme U3

//variables prenant la valeur des projections sur les axes x et y
float projX = 0;
float projY = 0;

void setup() {
  //déclarations des pins
  pinMode(x, INPUT);
  pinMode(y, INPUT);
  pinMode(z, INPUT);
  for (int i=2; i<14; i++) {
    pinMode(i, OUTPUT); }
  //
  delay(1000);
  SeqAllumage(); //signale la mise en route du module
  
  delay(1000);
  initVect(); //mesure de l'accélération à l'allumage
  x0 = vecteurInit[0];
  y0 = vecteurInit[1];
  z0 = vecteurInit[2];

  nU1 = sqrt(x0*x0 + y0*y0 + z0*z0); 
  nU2 = sqrt(y0*y0 + x0*x0); 
  nU3 = sqrt((y0*y0 + x0*x0 + z0*z0)*(x0*x0 + y0*y0));
  
  SetVectors(x0, y0, z0); //construit les trois vecteurs de la nouvelle base
  SeqInit(); //initialisation faite
}


void loop() {
  //on mesure
  mesure[0] = (float)analogRead(x) - x0;
  mesure[1] = (float)analogRead(y) - y0;
  mesure[2] = (float)analogRead(z) - z0;
  //
  
  //projection sur la bonne base
  projX = projection(2);
  projY = projection(3);
  //
  
  /*
   * allumage des leds sur l'axe X
   */
  if (projX <= -acc1) { //si c'est négatif
    digitalWrite(lednX[0], LOW);
    if (abs(projX) >= acc2) {
      digitalWrite(lednX[1], LOW);
      if (abs(projX) >= acc3) {
        digitalWrite(lednX[2], LOW);
      }
      else if (abs(projX) < acc3) {
        digitalWrite(lednX[2], HIGH);
      }
    }
    else if (abs(projX) < acc2) {
      digitalWrite(lednX[1], HIGH);
      digitalWrite(lednX[2], HIGH);
    }
  }
  else if (projX >= acc1) { //si c'est positif
    digitalWrite(ledpX[0], LOW);
    if (abs(projX) >= acc2) {
      digitalWrite(ledpX[1], LOW);  
      if (abs(projX) >= acc3) {
        digitalWrite(ledpX[2], LOW);
      }
      else if (abs(projX) < acc3) {
        digitalWrite(ledpX[2], HIGH);
      }
    }
    else if (abs(projX) < acc2) {
      digitalWrite(ledpX[1], HIGH);
      digitalWrite(ledpX[2], HIGH);
    }
  }
          
  else if (projX > -acc1 and projX < acc1) { //si la valeur n'est pas suffisamment grande
    digitalWrite(lednX[0], HIGH);
    digitalWrite(lednX[1], HIGH);
    digitalWrite(lednX[2], HIGH);
    digitalWrite(ledpX[0], HIGH);
    digitalWrite(ledpX[1], HIGH);
    digitalWrite(ledpX[2], HIGH);
  }
  //==================================================

  /*
   * allumage des leds sur l'axe Y
   */
  if (projY <= -acc1) { //si c'est négatif
    digitalWrite(lednY[0], LOW);
    if (abs(projY) >= acc2) {
      digitalWrite(lednY[1], LOW);
      if (abs(projY) >= acc3) {
        digitalWrite(lednY[2], LOW);
      }
      else if (abs(projY) < acc3) {
        digitalWrite(lednY[2], HIGH);
      }
    }
    else if (abs(projY) < acc2) {
      digitalWrite(lednY[1], HIGH);
      digitalWrite(lednY[2], HIGH);
    }
  }
  else if (projY >= acc1) { //si c'est positif
    digitalWrite(ledpY[0], LOW);
    if (abs(projY) >= acc2) {
      digitalWrite(ledpY[1], LOW);
      if (abs(projY) >= acc3) {
        digitalWrite(ledpY[2], LOW);
      }
      else if (abs(projY) < acc3) {
        digitalWrite(ledpY[2], HIGH);
      }
    }
    else if (abs(projY) < acc2) {
      digitalWrite(ledpY[1], HIGH);
      digitalWrite(ledpY[2], HIGH);
    }
  }

  else if (projY > -acc1 and projY < acc1) { //si la valeur n'est pas suffisamment grande
    digitalWrite(lednY[0], HIGH);
    digitalWrite(lednY[1], HIGH);
    digitalWrite(lednY[2], HIGH);
    digitalWrite(ledpY[0], HIGH);
    digitalWrite(ledpY[1], HIGH);
    digitalWrite(ledpY[2], HIGH);
  }
  //================================================
  delay(10); //for stability
}
//fin du void()


void initVect() {
  /*
   * initialise la mesure d'accélération avec une moyenne de la mesure
   */
  float meanX = 0;
  float meanY = 0;
  float meanZ = 0;
  float count = 15;
  for (int i = 0; i<count; i++) {
    meanX = meanX + (float)analogRead(x);
    meanY = meanY + (float)analogRead(y);
    meanZ = meanZ + (float)analogRead(z);
    delay(100);
  }
  vecteurInit[0] = meanX/count;
  vecteurInit[1] = meanY/count;
  vecteurInit[2] = meanZ/count;
}

void SetVectors(float xSet, float ySet, float zSet) {
  /*
   * construit les vecteurs de la nouvelle base
   */
  U1[0] = xSet/nU1;
  U2[0] = ySet/nU2;
  U3[0] = xSet*zSet/nU3;
  U1[1] = ySet/nU1;
  U2[1] = -xSet/nU2;
  U3[1] = ySet*zSet/nU3;
  U1[2] = zSet/nU1;
  U2[2] = 0;
  U3[2] = -(xSet*xSet + ySet*ySet)/nU3;
}

void SeqAllumage() {
  /*
   * séquence d'allumage des leds à la mise en route
   */
  for (int i = 0; i<19; i++) {
    digitalWrite(led1[i], LOW);
    digitalWrite(led2[i], LOW);
    digitalWrite(led3[i], LOW);
    delay(170);
    digitalWrite(led1[i], HIGH);
    digitalWrite(led2[i], HIGH);
    digitalWrite(led3[i], HIGH);
  }
}

void SeqInit() {
  /*
   * séquence d'allumage des leds à l'initialisation
   */
  for (int i=0; i<4; i++) {
    digitalWrite(ledpX[2], LOW);
    digitalWrite(ledpY[2], LOW);
    digitalWrite(lednX[2], LOW);
    digitalWrite(lednY[2], LOW);
    delay(70);
    digitalWrite(ledpX[2], HIGH);
    digitalWrite(ledpY[2], HIGH);
    digitalWrite(lednX[2], HIGH);
    digitalWrite(lednY[2], HIGH);
    delay(220);
  }
}

float projection(int vecteurUnitaire) {
  /*
   * projection sur le vecteur choisi de la mesure de l'accélération
   */
  float scalar_product = 0;
  if (vecteurUnitaire == 1) {
    for (int i = 0; i<=2; i++) {
      scalar_product = scalar_product + mesure[i]*U1[i];
    }
    return(scalar_product);
  }
  if (vecteurUnitaire == 2) {
    for (int i = 0; i<=2; i++) {
      scalar_product = scalar_product + mesure[i]*U2[i];
    }
    return(scalar_product);
  }
  else if (vecteurUnitaire == 3) {
    for (int i = 0; i<=2; i++) {
      scalar_product = scalar_product + mesure[i]*U3[i];
    }
    return(scalar_product);
  }
}

