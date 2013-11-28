#include <SD.h>

static File logger;
static boolean i_work = false;

void setup()
{
    char fname[16];
    uint8_t fnum = 0;

    analogReference(INTERNAL); // 2.56 V)
    Serial.begin(9600);

    pinMode(13, OUTPUT);
    pinMode(10, OUTPUT);
    if (!SD.begin(10)) {
        Serial.println("Erro no cartao SD");
        return;
    }

    do {
        sprintf(fname, "logger%d.txt", ++fnum);
    } while (SD.exists(fname));

    logger = SD.open(fname, FILE_WRITE);
    if (!logger) {
        Serial.println("Erro abrindo arquivo");
        return;
    }

    Serial.println("Funfando");
    Serial.println(fname);
    i_work = true;
}

// 18:57 já tinha caído

static uint8_t flushme = 0;
void loop()
{
    while (Serial.available()) {
        char c = Serial.read();
        logger.print(c);
    }
    if (i_work) {
        unsigned long x = analogRead(A0);
        float f = (float) (x / 1023.) * 1.1 * (14.7 + 470.) / 14.7;
        unsigned long t = millis();
        t /= 500;
        logger.print(t);
        logger.print(" ");
        logger.println(f);
        delay(500);
        if (++flushme == 60) {
            logger.flush();
            flushme = 0;
        }
    }
}

