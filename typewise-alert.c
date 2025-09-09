#include "typewise-alert.h"
#include <stdio.h>

typedef struct {
  int lower;
  int upper;
} Limits;

static const Limits coolingLimits[] = {
  [PASSIVE_COOLING]  = {0, 35},
  [HI_ACTIVE_COOLING]= {0, 45},
  [MED_ACTIVE_COOLING]= {0, 40}
};

BreachType inferBreach(double value, double lower, double upper) {
  return (value < lower) ? TOO_LOW :
         (value > upper) ? TOO_HIGH : NORMAL;
}

BreachType classifyTemperatureBreach(CoolingType type, double tempC) {
  return inferBreach(tempC, coolingLimits[type].lower, coolingLimits[type].upper);
}

void sendToController(BreachType breachType) {
  printf("%x : %x\n", 0xfeed, breachType);
}

void sendToEmail(BreachType breachType) {
  const char* recipient = "a.b@c.com";
  const char* messages[] = {
    [TOO_LOW]  = "Hi, the temperature is too low",
    [TOO_HIGH] = "Hi, the temperature is too high"
  };
  if(breachType != NORMAL) {
    printf("To: %s\n%s\n", recipient, messages[breachType]);
  }
}

void checkAndAlert(AlertTarget target, BatteryCharacter battery, double tempC) {
  BreachType breach = classifyTemperatureBreach(battery.coolingType, tempC);
  (target == TO_CONTROLLER) ? sendToController(breach) : sendToEmail(breach);
}
