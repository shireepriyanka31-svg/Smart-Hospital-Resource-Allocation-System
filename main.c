#include <stdio.h>
#include <stdlib.h>

#define NUM_WARDS        4
#define MAX_BEDS        20
#define MAX_PATIENTS   100
#define NUM_SPECIALTIES  4

void registerPatient();
void displayBill(int i);
void savePatientRecord(int i);

int wardCapacity[NUM_WARDS] = {20, 10, 10, 5};
int bedOccupancy[NUM_WARDS][MAX_BEDS] = {0};
char wardName[NUM_WARDS][40] = {
    "General Ward", "Paediatric Ward", "Surgical Ward", "ICU (Intensive Care Unit)"};
int specialtyID[NUM_SPECIALTIES] = {1, 2, 3, 4};
char specialtyName[NUM_SPECIALTIES][30] = {
    "General Practice (OPD)", "Paediatrics", "Cardiology", "Neurology"};
float baseFee[NUM_SPECIALTIES] = {1500.00, 2500.00, 4500.00, 5000.00};
int wardIDArr[NUM_WARDS] = {1, 2, 3, 4};
float wardRate[NUM_WARDS] = {3000.00, 6000.00, 12000.00, 25000.00};

int patientCount = 0;
char patientName[MAX_PATIENTS][50];
int patientAge[MAX_PATIENTS];
int urgencyLevel[MAX_PATIENTS];
int isAdmitted[MAX_PATIENTS];
int specialtyIndex[MAX_PATIENTS];
int wardIndex[MAX_PATIENTS];
int bedNumber[MAX_PATIENTS];
int daysAdmitted[MAX_PATIENTS];

int queueCount[NUM_SPECIALTIES] = {0};
int waitTime[MAX_PATIENTS];
float surcharge[MAX_PATIENTS];
float wardCost[MAX_PATIENTS];
float grossTotal[MAX_PATIENTS];
float discount[MAX_PATIENTS];
float finalAmount[MAX_PATIENTS];

int findFreeBed(int wIdx);
int calcWaitTime(int sIdx);
float calcSurcharge(int urgency, float base);
float calcWardCost(int days, float rate);
float calcDiscount(int age, float gross);

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

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Exiting...\n");
            break;
        }

        switch (choice) {
            case 1:
                registerPatient();
                break;
            case 2:
                printf("Display bed occupancy pending implementation.\n");
                break;
            case 3:
                printf("Triage display pending implementation.\n");
                break;
            case 4:
                printf("Summary report pending implementation.\n");
                break;
            case 5:
                printf("Saving state and exiting system...\n");
                break;
            default:
                printf("Invalid choice, please try again.\n");
        }
    } while (choice != 5);

    return 0;
}

void registerPatient() {
    if (patientCount >= MAX_PATIENTS) {
        printf("Patient records are full!\n");
        return;
    }

    int i = patientCount;

    printf("\nEnter Patient Name: ");
    scanf(" %[^\n]", patientName[i]);

    printf("Enter Patient Age: ");
    scanf("%d", &patientAge[i]);

    printf("Enter Urgency Level (1=Normal, 2=Urgent, 3=Critical): ");
    scanf("%d", &urgencyLevel[i]);

    printf("\nAvailable Specialties:\n");
    for (int s = 0; s < NUM_SPECIALTIES; s++) {
        printf("  %d. %-24s (Fee: %.2f)\n", specialtyID[s], specialtyName[s], baseFee[s]);
    }
    int sID;
    printf("Choose Specialty ID (1-4): ");
    scanf("%d", &sID);
    int sIdx = sID - 1;

    printf("Is the patient admitted to a ward? (1=Yes, 0=No): ");
    scanf("%d", &isAdmitted[i]);

    int wIdx = -1, bed = -1, days = 0;

    if (isAdmitted[i] == 1) {
        printf("\nAvailable Wards:\n");
        for (int w = 0; w < NUM_WARDS; w++) {
            printf("  %d. %-28s (Rate: %.2f/day)\n", wardIDArr[w], wardName[w], wardRate[w]);
        }
        int wID;
        printf("Choose Ward ID (1-4): ");
        scanf("%d", &wID);
        wIdx = wID - 1;

        bed = findFreeBed(wIdx);
        if (bed == -1) {
            printf("Sorry, no free beds in %s. Registering as Outpatient instead.\n", wardName[wIdx]);
            isAdmitted[i] = 0;
            wIdx = -1;
        } else {
            bedOccupancy[wIdx][bed] = 1;
            printf("Enter Number of Days Admitted: ");
            scanf("%d", &days);
        }
    }

    specialtyIndex[i] = sIdx;
    wardIndex[i]      = wIdx;
    bedNumber[i]      = bed;
    daysAdmitted[i]   = days;

    waitTime[i]    = calcWaitTime(sIdx);
    surcharge[i]   = calcSurcharge(urgencyLevel[i], baseFee[sIdx]);
    wardCost[i]    = (isAdmitted[i] == 1) ? calcWardCost(days, wardRate[wIdx]) : 0.0f;
    grossTotal[i]  = baseFee[sIdx] + surcharge[i] + wardCost[i];
    discount[i]    = calcDiscount(patientAge[i], grossTotal[i]);
    finalAmount[i] = grossTotal[i] - discount[i];

    queueCount[sIdx]++;
    patientCount++;

    savePatientRecord(i);
}

void displayBill(int i) {
    printf("\n================ PATIENT BILL ================\n");
    printf("Patient Name  : %s\n", patientName[i]);
    printf("Age           : %d\n", patientAge[i]);
    printf("Specialty     : %s\n", specialtyName[specialtyIndex[i]]);
    printf("Base Fee      : $%.2f\n", baseFee[specialtyIndex[i]]);
    printf("Surcharge     : $%.2f\n", surcharge[i]);
    printf("Ward Cost     : $%.2f\n", wardCost[i]);
    printf("Gross Total   : $%.2f\n", grossTotal[i]);
    printf("Discount      : -$%.2f\n", discount[i]);
    printf("----------------------------------------------\n");
    printf("Final Amount  : $%.2f\n", finalAmount[i]);
    printf("Est. Wait Time: %d mins\n", waitTime[i]);
    printf("==============================================\n");
}
