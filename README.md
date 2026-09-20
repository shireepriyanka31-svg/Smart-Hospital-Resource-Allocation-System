# Smart Hospital & Resource Allocation System

A menu-driven console application written in C that simulates a hospital's daily patient intake, bed allocation, emergency triage sorting, and medical billing process.

Built for **CSC 1012 – Introduction to Computer Programming**, University of Sri Jayewardenepura.

## Features

- **Patient Registration** — captures name, age, urgency level, chosen specialty, and optional ward admission
- **Automated Billing** — calculates consultation fees, emergency surcharges, ward stay costs, and age-based subsidy discounts
- **Bed Allocation** — tracks bed availability per ward using a 2D occupancy matrix, assigns the first free bed automatically
- **Triage Priority Sorting** — displays registered patients ordered by urgency level (Critical → Urgent → Normal)
- **Summary Reports** — patient counts by urgency, total revenue, total discounts granted, bed occupancy percentages, and the highest-paying patient
- **File Handling** — bed status persists between runs (`beds_status.txt`); every billing record is permanently logged (`patient_records.txt`)

## How to Compile & Run

```bash
gcc main.c -o hospital.exe
./hospital
```

On Windows:

```bash
gcc main.c -o hospital.exe
hospital.exe
```

## Menu Options

```
1. Register New Patient
2. Display Bed Occupancy
3. Display Patients by Priority (Triage)
4. Generate Summary Report
5. Save & Exit
```

## Data Structures

This project uses **parallel arrays** (no structs) to represent all hospital and patient data:

- Lookup arrays for the 4 doctor specialties and 4 hospital wards (fees, rates, capacities)
- `bedOccupancy[4][20]` — a 2D array tracking bed availability (`0` = free, `1` = occupied)
- Patient data spread across multiple parallel arrays (name, age, urgency, billing amounts, etc.), all indexed by the same patient index `i`

## Billing Formulas

| Item | Formula |
|---|---|
| Waiting Time | Queue count for specialty × average consultation time |
| Emergency Surcharge | 0% (Normal), 20% of base fee (Urgent), 50% of base fee (Critical) |
| Ward Stay Cost | Days admitted × ward's daily rate |
| Age Subsidy Discount | 15% of gross total, if age &lt; 5 or age &gt; 65 |
| Final Payable Amount | Gross total − discount |

## Project Structure

```
.
├── main.c                # main source file
├── .gitignore            # excludes compiled binaries and generated data files
├── beds_status.txt        # generated at runtime (bed occupancy state)
├── patient_records.txt    # generated at runtime (billing history log)
└── README.md
```

## Author

Priyanka Shireetharran (AS20250645)
BSc (General) Degree, First Year
Faculty of Applied Sciences, University of Sri Jayewardenepura
