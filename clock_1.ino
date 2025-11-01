#include <LiquidCrystal.h>

LiquidCrystal lcd(12,11,5,4,3,2);

const int BUT_1 = 8, BUT_2 = 9, BUT_3 = 10, delayTimeClock = 1000, delayTimeButtons = 125, delayTimeSetTime = 300;
unsigned long currentMillis, previousMillisClock = 0, previousMillisButtons = 0, previousMillisSetTime = 0;

int secs = 0, mins = 0, hours = 0, pressButts[3];

/*
0 = No settaggio tempo
1 = Settaggio ora
2 =    ""     minuti
3=     ""     secondi
*/
int isSetTime = 0;

bool blinkSetTime = false;

void resetLCD(){
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,0);
}

//CHIAMARE SOLO 1 VOLTA AL SECONDO, ALTRIMENTI L'OROLOGIO NON OROLOGIA
void countTime(){
  //Time manager


  //Non conto più i secondi se li sto settando
  if(isSetTime !=3){
    //If secs = 60
    if((secs+1) == 60){
      //if mins = 60
      if((mins+1) == 60){
        //if hours == 24
        if(hours+1 == 24){
          hours = 0;
          mins = 0;
          secs = 0;
        }
        else{
          hours++;
          mins = 0;
          secs = 0;
        }
      }
      else{
        mins++;
        secs =0;
      }
    }
    else{
      secs++;
    }
  }

}

void printTime(int hour, int min, int sec){
  char buffer[9];

  if(isSetTime == 0){
    sprintf(buffer, "%02d:%02d:%02d", hour, min, sec);
  }
  else{
    if(currentMillis - previousMillisSetTime >= delayTimeSetTime){
      previousMillisSetTime = currentMillis;

      if(blinkSetTime){
        blinkSetTime = false;
          sprintf(buffer, "%02d:%02d:%02d", hour, min, sec);
      }
      else{
        blinkSetTime = true;
        switch(isSetTime){
          case 1:
            sprintf(buffer, "  :%02d:%02d", min, sec);
            
          break;

          case 2:
            sprintf(buffer, "%02d:  :%02d", hour, sec);
          break;

          case 3:
            sprintf(buffer, "%02d:%02d:  ", hour, min);
          break;
        }  
      }
  
    }
  }

  lcd.setCursor(4,0);
  lcd.print(buffer);

}

void setup() {
  lcd.begin(16,2);

  //Button init, 8,9,10
  for (int i=BUT_1;i<=BUT_3;i++){
    pinMode(i, INPUT);
  }

}

void loop() {
  currentMillis = millis();

  if(currentMillis - previousMillisButtons >= delayTimeButtons){
    previousMillisButtons = currentMillis;

    for(int i=BUT_1;i<=BUT_3;i++){
      pressButts[i-BUT_1] = digitalRead(i);
    }

    //Button 1
    if(pressButts[0] == HIGH){
      if(isSetTime !=0){
        //Seve a far comparire subito il numero cambiato, senza aspettare il delay del blink
        previousMillisSetTime = delayTimeSetTime;
        switch(isSetTime){
          case 1:
            if(hours-1 == -1){
              hours = 23;
            }
            else{
              hours--;
            }
            
          break;

          case 2:
            if(mins-1 == -1){
              mins = 59;
            }
            else{
              mins--;
            }
          break;

          case 3:
            if(secs-1 == -1){
              secs = 59;
            }
            else{
              secs--;
            }          
          break;
        }

        printTime(hours, mins, secs);
      }
    }

    //Button 3
    else if(pressButts[2] == HIGH){
      if(isSetTime !=0){

        //Seve a far comparire subito il numero cambiato, senza aspettare il delay del blink
        previousMillisSetTime = delayTimeSetTime;
        switch(isSetTime){
          case 1:
            if(hours+1 == 24){
              hours = 0;
            }
            else{
              hours++;
            }
            
          break;

          case 2:
            if(mins+1 == 60){
              mins = 0;
            }
            else{
              mins++;
            }
          break;

          case 3:
            if(secs+1 == 60){
              secs = 0;
            }
            else{
              secs++;
            }          
          break;
        }
        printTime(hours, mins, secs);

      }
    }

    //Button 2
    else if(pressButts[1] == HIGH){
      if(isSetTime == 0){
        isSetTime = 1;
      }
      else if(isSetTime +1 <4){
        isSetTime++;
      }
      else{
        isSetTime = 0;
      }
    }

  }
  //Se sono passati 1000ms(1s)
  if(currentMillis - previousMillisClock >= delayTimeClock){
      previousMillisClock = currentMillis;

      countTime();
      printTime(hours, mins, secs);
  }
}
