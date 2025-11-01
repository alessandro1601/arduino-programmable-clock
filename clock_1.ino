#include <LiquidCrystal.h>

LiquidCrystal lcd(12,11,5,4,3,2);

const int BUT_1 = 8, BUT_2 = 9, BUT_3 = 10, delayTimeClock = 1000, delayTimeButtons = 125, delayTimeSetTime = 300;
unsigned long currentMillis, previousMillisClock = 0, previousMillisButtons = 0, previousMillisSetTime = 0;

int secs = 0, mins = 0, hours = 0, day = 0, month =0 , year = 2000, pressButts[3];

/*
0 = No settaggio tempo
1 = Settaggio ora
2 =    ""     minuti
3=     ""     secondi
*/
int isSetTime = 0, isSetDate = 0;

bool blinkSetTime = false, blinkSetDate;

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
          countDate();
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

void countDate(){
  if(day+1 > 29){
    //February
    if(month == 2){
      //Check for leap years
      if(year % 4 == 0){
          day++;
      }
      else if(year % 100 == 0){
        if(year % 400 == 0){
          day++;
        }
        else{
          day = 0;
          month++;          
        }
      }
      else{
        day = 0;
        month++;
      }
    }
    else{
      day++;
    }

  }
  else if(day+1 == 31){
    if(month == 4 || month == 6 || month == 9 || month == 11){
      day = 0;
      month++;
    }
  }
  else if(day+1 == 32){
      day = 0;
      if(month+1 !=13){
        month++;

      }
      else{
        day = 0;
        month = 0;
        year++;
      }
    
  }
  else{
    day++;
  }

  printDate();

}

void printTime(){
  char buffer[9];

  if(isSetTime == 0){
    sprintf(buffer, "%02d:%02d:%02d", hours, mins, secs);
  }
  else{
    if(currentMillis - previousMillisSetTime >= delayTimeSetTime){
      previousMillisSetTime = currentMillis;

      if(blinkSetTime){
        blinkSetTime = false;
          sprintf(buffer, "%02d:%02d:%02d", hours, mins, secs);
      }
      else{
        blinkSetTime = true;
        switch(isSetTime){
          case 1:
            sprintf(buffer, "  :%02d:%02d", mins, secs);
            
          break;

          case 2:
            sprintf(buffer, "%02d:  :%02d", hours, secs);
          break;

          case 3:
            sprintf(buffer, "%02d:%02d:  ", hours, mins);
          break;
        }  
      }
  
    }
  }

  lcd.setCursor(4,0);
  lcd.print(buffer);

}

void printDate(){
  char buffer[9];
  sprintf(buffer, "%02d/%02d/%04d", day, month, year);


  lcd.setCursor(3,1);
  lcd.print(buffer);
}

void setup() {
  lcd.begin(16,2);

  //Button init, 8,9,10
  for (int i=BUT_1;i<=BUT_3;i++){
    pinMode(i, INPUT);
  }
  printDate();

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

        printTime();
      }

      /*
      else if(isSetDate !=0){
        //Seve a far comparire subito il numero cambiato, senza aspettare il delay del blink
        previousMillisSetTime = delayTimeSetTime;
        switch(isSetDate){
          case 1:
            if(day-1 == -1){
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
      }
      */
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
        printTime();

      }
    }

    //Button 2
    else if(pressButts[1] == HIGH){
      if(isSetTime == 0){
        isSetTime = 1;
      }
      else if(isSetTime +1 <4){
        isSetTime++;
        //previousMillisSetTime = delayTimeSetTime;
        blinkSetTime = false;
        printTime();
      }
      else if(isSetTime == 3){
        isSetTime = 0;
        //isSetDate = 1;
      }

      else if(isSetDate +1 < 4){
        //isSetDate++;
        //blinkSetDate = false;
        //printDate();
      }
      else{
        isSetDate = 0;

      }
    }

  }
  //Se sono passati 1000ms(1s)
  if(currentMillis - previousMillisClock >= delayTimeClock){
      previousMillisClock = currentMillis;

      countTime();
      printTime();
  }
}
