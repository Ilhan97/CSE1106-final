#include <stdio.h>
#include <pigpio.h>

#define PIN 17

int main(void)
{
    double minimum, maximum;
    double temperature, total = 0;
    int count;

    printf("Enter minimum temperature: ");
    scanf("%lf", &minimum);

    printf("Enter maximum temperature: ");
    scanf("%lf", &maximum);

    if (minimum > maximum) {
        printf("Minimum cannot exceed maximum.\n");
        return 1;
    }

    printf("How many readings? ");
    scanf("%d", &count);

    if (count <= 0) {
        printf("You need at least one reading.\n");
        return 1;
    }

    if (gpioInitialise() < 0) {
        printf("GPIO setup failed.\n");
        return 1;
    }

    gpioSetMode(PIN, PI_OUTPUT);
    gpioWrite(PIN, 0);

    FILE *file = fopen("data/temperatures.txt", "a");

    if (file == NULL) {
        printf("Could not open file.\n");
        gpioTerminate();
        return 1;
    }

    for (int j = 1; j <= count; j++) {
        printf("Enter temperature %d: ", j);
        scanf("%lf", &temperature);

        total = total + temperature;

        if (temperature < minimum || temperature > maximum) {
            printf("%-12.1f %-15s %s\n", temperature, "WARNING", "ON");
            gpioWrite(PIN, 1);
            fprintf(file, "%.1f F - WARNING\n", temperature);
        } else {
            printf("%-12.1f %-15s %s\n", temperature, "NORMAL", "OFF");
            gpioWrite(PIN, 0);
            fprintf(file, "%.1f F - NORMAL\n", temperature);
        }
    }

    printf("Average: %.1f F\n", total / count);
    fclose(file);

    printf("Press Enter to finish.\n");
    getchar();  
    getchar(); 
    gpioWrite(PIN, 0);
    gpioTerminate();

    return 0;
}

