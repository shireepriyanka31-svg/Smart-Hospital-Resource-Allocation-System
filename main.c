#include <stdio.h>
#include <stdlib.h>

int main()
{
    int choice;
    do {
        printf("\n============ SMART HOSPITAL SYSTEM MENU ============\n");
        printf("1. Register New Patient\n");
        printf("2. Display Bed Occupancy\n");
        printf("3. Display Patients by Priority (Triage)\n");
        printf("4. Generate Summary Report\n");
        printf("5. Save & Exit\n");
        printf("======================================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
    }
    while (choice != 5);
    return 0;
}
