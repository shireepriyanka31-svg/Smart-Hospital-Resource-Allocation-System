#include <stdio.h>
#include <stdlib.h>

#define NUM_WARDS        4
#define MAX_BEDS        20
#define MAX_PATIENTS   100
#define NUM_SPECIALTIES  4

void registerPatient();
void displayBill(int i);
void registerPatient();
int findFreeBed(int wIdx);
int calcWaitTime(int sIdx);
float calcSurcharge(int urgency, float base);
float calcWardCost(int days, float rate);
float calcDiscount(int age, float gross);
void savePatientRecord(int i);
void displayBedOccupancy();
void sortAndDisplayByPriority();
void initBeds();
void generateReport() ;

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
int   dailyCap[NUM_SPECIALTIES]      = {30, 20, 12, 10};
int   queueCount[NUM_SPECIALTIES]    = {0, 0, 0, 0};


int findFreeBed(int wardIdx);
void registerPatient();
float calcWaitTime(int specialtyIdx);
float calcSurcharge(int urgency, float fee);
float calcWardCost(int days, float rate);
float calcDiscount(int age, float gross);
int displayBill(int i);
void sortAndDisplayByPriority();
void generateReport();
void saveBedsStatus();
void loadBedsStatus();
void savePatientRecord(int i);
void important();

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
                displayBedOccupancy();
                break;
            case 3:
                sortAndDisplayByPriority();
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

int findFreeBed(int wIdx) {
    if (wIdx < 0 || wIdx >= NUM_WARDS) return -1;
    for (int b = 0; b < wardCapacity[wIdx]; b++) {
        if (bedOccupancy[wIdx][b] == 0) return b;
    }
    return -1;
}

int calcWaitTime(int sIdx) {
    return queueCount[sIdx] * 15;
}

float calcSurcharge(int urgency, float base) {
    if (urgency == 2) return base * 0.20f;
    if (urgency == 3) return base * 0.50f;
    return 0.0f;
}

float calcWardCost(int days, float rate) {
    return days * rate;
}

float calcDiscount(int age, float gross) {
    if (age >= 60 || age <= 12) return gross * 0.10f;
    return 0.0f;
}

void savePatientRecord(int i) {
    FILE *file = fopen("patient_records.txt", "a");
    if (file == NULL) return;
    fprintf(file, "%s,%d,%d,%.2f\n", patientName[i], patientAge[i], urgencyLevel[i], finalAmount[i]);
    fclose(file);
}

void displayBedOccupancy() {
    printf("\n---------------- BED OCCUPANCY STATUS ----------------\n");
    for (int w = 0; w < NUM_WARDS; w++) {
        int occupied = 0;
        for (int b = 0; b < wardCapacity[w]; b++) {
            if (bedOccupancy[w][b] == 1) occupied++;
        }
        printf("%s: %d / %d beds occupied\n", wardName[w], occupied, wardCapacity[w]);
        printf("  Beds: ");
        for (int b = 0; b < wardCapacity[w]; b++) {
            printf("%d ", bedOccupancy[w][b]);
        }
        printf("\n");
    }
    printf("--------------------------------------------------------\n");
}

void sortAndDisplayByPriority(void) {
    int order[MAX_PATIENTS];
    for (int i = 0; i < patientCount; i++) {
        order[i] = i;
    }

    for (int a = 0; a < patientCount - 1; a++) {
        for (int b = 0; b < patientCount - 1 - a; b++) {
            if (urgencyLevel[order[b]] < urgencyLevel[order[b + 1]]) {
                int temp = order[b];
                order[b] = order[b + 1];
                order[b + 1] = temp;
            }
        }
    }

    printf("\n---------------- PATIENTS BY PRIORITY ----------------\n");
    for (int k = 0; k < patientCount; k++) {
        int i = order[k];
        printf("%d. PAT-%d | %-20s | Urgency Level %d\n",
               k + 1, 1000 + i + 1, patientName[i], urgencyLevel[i]);
    }
    printf("-------------------------------------------------------\n");
}

void important() {
    initBeds();
    loadBedsStatus();
    return 0;
}

void initBeds(void) {
    for (int w = 0; w < NUM_WARDS; w++) {
        for (int b = 0; b < MAX_BEDS; b++) {
            bedOccupancy[w][b] = 0;
        }
    }
}

void loadBedsStatus(void) {
    FILE *fp = fopen("beds_status.txt", "r");
    if (fp == NULL) {
        return; 
    }
    for (int w = 0; w < NUM_WARDS; w++) {
        for (int b = 0; b < MAX_BEDS; b++) {
            fscanf(fp, "%d", &bedOccupancy[w][b]);
        }
    }
    fclose(fp);
}

float calcWaitTime(int specialtyIdx) {
    return queueCount[specialtyIdx] * consultTime[specialtyIdx];
}

float calcSurcharge(int urgency, float fee) {
    if (urgency == 2) return fee * 0.20f;
    if (urgency == 3) return fee * 0.50f;
    return 0.0f;
}

float calcWardCost(int days, float rate) {
    return days * rate;
}

float calcDiscount(int age, float gross) {
    if (age < 5 || age > 65) return gross * 0.15f;
    return 0.0f;
}

void generateReport() {
    int countLevel1 = 0, countLevel2 = 0, countLevel3 = 0;
    float totalRevenue = 0, totalDiscount = 0;
    float highestBill = -1;
    int highestIndex = -1;

    for (int i = 0; i < patientCount; i++) {
        if (urgencyLevel[i] == 1) countLevel1++;
        else if (urgencyLevel[i] == 2) countLevel2++;
        else if (urgencyLevel[i] == 3) countLevel3++;

        totalRevenue  += finalAmount[i];
        totalDiscount += discount[i];

        if (finalAmount[i] > highestBill) {
            highestBill = finalAmount[i];
            highestIndex = i;
        }
    }

    printf("\n================ HOSPITAL SUMMARY REPORT ================\n");
    printf("Total Patients Registered : %d\n", patientCount);
    printf("  Level 1 (Normal)   : %d\n", countLevel1);
    printf("  Level 2 (Urgent)   : %d\n", countLevel2);
    printf("  Level 3 (Critical) : %d\n", countLevel3);
    printf("Total Revenue Earned       : LKR %.2f\n", totalRevenue);
    printf("Total Discounts Granted    : LKR %.2f\n", totalDiscount);

    printf("\nBed Occupancy Percentage:\n");
    for (int w = 0; w < NUM_WARDS; w++) {
        int occupied = 0;
        for (int b = 0; b < wardCapacity[w]; b++) {
            if (bedOccupancy[w][b] == 1) occupied++;
        }
        float percent = (occupied * 100.0f) / wardCapacity[w];
        printf("  %-28s: %.1f%% occupied\n", wardName[w], percent);
    }

    if (highestIndex != -1) {
        printf("\nHighest-Paying Patient: %s (LKR %.2f)\n", patientName[highestIndex], highestBill);
    } else {
        printf("\nNo patients registered yet.\n");
    }
    printf("===========================================================\n");
}


