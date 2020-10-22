#include "mbed.h"
#include "USBHostMSD.h"

#define fech 40000      // Fréquence d'échantillonnage souhaitée
#define TAILLE_TAB (fech/10)    // Pour stocker 1/10 s du signal
// Donc fmin signal = 10 Hz

Serial pc(USBTX, USBRX);

AnalogIn entree(PC_3); // A2 connecteur Arduino
PwmOut sortie(PE_9); // D6 Arduino
Ticker drive_ADC ;
DigitalIn button(USER_BUTTON);

// Filtre RIF passe_bas fc = 1500 Hz généré avec Octave Fe réelle = 40 kHz
// Fe = 2 dans Octave --> Fc Octave = 1500/(FE/2)= 0.075
// coeff = fir1(20,0.075) ; 21 = nb de coeffients dans coeff

#define NB_COEFF 21 // Nombre de coefficients du filtre RIF
float coeff_filtre[NB_COEFF]= {0.0026603,0.0045069,0.0091798,
                               0.017556,0.029809,0.045263,0.062429,0.079237,0.093403,
                               0.10286,0.10619,0.10286,0.093403,0.079237,0.062429,
                               0.045263, 0.029809,0.017556, 0.0091798,0.0045069,0.0026603
                              } ;


float in[TAILLE_TAB] ;

void acquisition_ADC()
{
    static int i = 0 ;
    in[i++] = 3.3f*entree.read();
    if (i == TAILLE_TAB) i = 0 ;
}

void ECRIT_USB_MSD(float *in_sig, float *out_sig, int taille)
{
    USBHostMSD msd("usb");

    pc.printf("Connectez la clef USB sur le connecteur prevu a cet effet\n");
    while(!msd.connect()) {
    }
    pc.printf("Clef USB connectee ne pas debrancher\n");
    pc.printf("Ecriture des donnees du signal d'entree sur la clef USB\n");
    FILE *fp = fopen("/usb/in.txt","w");
    if (fp != NULL) {
        fwrite(in_sig,sizeof(in_sig[0]),taille,fp);
        fclose(fp);
        pc.printf("Ecriture terminee\n");
    } else
        pc.printf("Impossible d'ouvrir le fichier\n");

    pc.printf("Ecriture des donnees du signal de sortie sur la clef USB\n");

    fp = fopen("/usb/out.txt","w");
    if (fp != NULL) {
        fwrite(out_sig,sizeof(out_sig[0]),taille,fp);
        fclose(fp);
        pc.printf("Ecriture terminee\n");
    } else
        pc.printf("Impossible d'ouvrir le fichier\n");
}

int main()
{
    float out[TAILLE_TAB] ;
    for (int i = 0; i < TAILLE_TAB ; i++ )
        out[i] = 0 ;
    drive_ADC.attach_us(&acquisition_ADC,1000000/fech);
    sortie.period_us(2000);
    sortie.write(0.5f) ; // rapport cyclique 1/2
    pc.printf("\033[2J"); // Sequence escape pour effacer la console
    pc.printf("\033[0;0H"); // Curseur en 0 ; 0
    pc.printf("Essai filtrage numerique temps differe fech = %d Hz\n",fech);
    pc.printf("Signal PWM test sur D6. Entree analogique a filtrer sur A2\n") ;
    pc.printf("Connecter D6 sur A2 ou un signal externe sur A2\n") ;
    pc.printf("Appuyez sur le bouton USER de la carte une fois fait \n");
    while(button.read() == 0 ) ;
    wait(1) ;   // Attendre une seconde pour remplir le buffer signal in[]
    drive_ADC.detach() ;
    // Convolution avec filtre RIF
    ??????????????
    ??????????????
    ??????????????
    ??????????????
    ECRIT_USB_MSD(in, out, TAILLE_TAB) ;
    while(1) ;
}
