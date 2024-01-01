#include <stdio.h>
#include <stdlib.h>

#define DAYS_IN_WEEK 7 // �@�P���Ѽ�
#define TIME_SLOTS 3   // �����ߤT�Ӯɬq
#define HOURLY_RATE 180 // ���`���~

typedef struct {
	char name[50];
	int workHours;                  // �u�@�ɼ�
	int schedule[DAYS_IN_WEEK][TIME_SLOTS]; // �@�P���ƯZ��
	int overtimeHours;              // �[�Z�ɼ�
	int overtimePay;                // �[�Z�O��
	int holidayPay;                 // �����~��
	int totalPay;                   // ��P�~��
} Employee;

// ��J���u�ƯZ���p
void inputEmployeeSchedule(Employee *employee) {
	printf("��J %s ���@�P�ƯZ���p�]�����ߡA1��ܤW�Z�A0��ܤ��W�Z�^:\n", employee->name);
	for (int day = 0; day < DAYS_IN_WEEK; ++day) {
		printf("§�� %d: ", day + 1);
		scanf("%d %d %d", &employee->schedule[day][0], &employee->schedule[day][1], &employee->schedule[day][2]);
	}
}

// �p���~��
void calculatePay(Employee *employee) {
	int totalWorkHours = 0;
	int weekendWorkHours = 0;
	int overtimeHours = 0;

	for (int day = 0; day < DAYS_IN_WEEK; ++day) {
		int dailyWorkHours = 0; // �Ω�O����Ѥu�@���ɼ�
		for (int slot = 0; slot < TIME_SLOTS; ++slot) {
			if (employee->schedule[day][slot] == 1) {
				dailyWorkHours += 4; // �C�Ӯɬq��4�p��
			}
		}

		if (day == 5 || day == 6) { // �p�G�O�P�����άP����
			weekendWorkHours += dailyWorkHours; // �֭p�P���u�@�ɼ�
		}

		if (dailyWorkHours > 8) {
			overtimeHours += dailyWorkHours - 8; // �p����W�L8�p�ɪ��[�Z�ɼ�
		}

		totalWorkHours += dailyWorkHours; // �֭p�`�u�@�ɼ�
	}

	int regularHours = (totalWorkHours > 40) ? 40 : totalWorkHours; // �зǤu��

	int holidayHours = 0; // ����u��
	if (weekendWorkHours > 8) { // �p�G�P���u�ɶW�L8�p��
		holidayHours = weekendWorkHours - 8; // �p�ⰲ��u��
	}

	int overtimePay = overtimeHours * HOURLY_RATE * 1.34; // �W�L�зǤu�ɪ��ɼƫ���1.34�����~�p��

	employee->workHours = regularHours;
	employee->overtimeHours = overtimeHours;
	employee->holidayPay = holidayHours * HOURLY_RATE;
	employee->overtimePay = overtimePay;
	employee->totalPay = (regularHours * HOURLY_RATE) + employee->overtimePay + employee->holidayPay;
}



// ����~�����
void displayPayroll(Employee *employee) {
	printf("\n���u�m�W�G%s\n", employee->name);
	printf("���`�u�ɡG%d �p��\n", employee->workHours);
	printf("�[�Z�ɼơG%d �p��\n", employee->overtimeHours);
	printf("�[�Z�O�ΡG%d ��\n", employee->overtimePay);
	printf("�����~��G%d ��\n", employee->holidayPay);
	printf("��P�~���G%d ��\n", employee->totalPay);
}

// �C�L�Z��
void printSchedule(Employee *employee) {
	printf("\n���u %s ���@�P�ƯZ���p:\n", employee->name);
	for (int day = 0; day < DAYS_IN_WEEK; ++day) {
		printf("�P�� %d: %d %d %d\n", day + 1, employee->schedule[day][0], employee->schedule[day][1], employee->schedule[day][2]);
	}
}

// �ק�Z��
void modifySchedule(Employee *employee) {
	int num, day, timeSlot;

	printf("�п�J�n���ɶ�������:");
	scanf("%d", &num);
	for (int i = 0; i < num; i++) {
		printf("\n�п�J�n�ק���u %s ���ƯZ�����(1-7)�M�ɬq(1-3)�G", employee->name);
		scanf("%d %d", &day, &timeSlot);
		printf("��J�s���ƯZ���p�]1��ܤW�Z�A0��ܤ��W�Z�^�G");
		scanf("%d", &employee->schedule[day - 1][timeSlot - 1]);
	}
}

int main() {
	int numEmployees;
	int option;

	printf("�п�J���u�H�ơG");
	scanf("%d", &numEmployees);

	Employee *employees = (Employee *)calloc(numEmployees, sizeof(Employee)); // Use calloc to initialize memory to zero

	// ��J���u�W�r�M�ƯZ���p
	for (int i = 0; i < numEmployees; ++i) {
		printf("��J�� %d ����u���m�W: ", i + 1);
		scanf("%s", employees[i].name);
		inputEmployeeSchedule(&employees[i]);
	}

	// �\����
	do {
		printf("\n��ܥ\��G\n");
		printf("1. �C�L�Z��\n");
		printf("2. �ק�Z��\n");
		printf("3. �C�L���u�~��\n");
		printf("4. �����{��\n");
		scanf("%d", &option);

		switch (option) {
		case 1:
			// �C�L�Z��
			for (int i = 0; i < numEmployees; ++i) {
				printSchedule(&employees[i]);
			}
			break;
		case 2:
			// �ק�Z��
			int i;
			printf("�п�J�n�ק諸���u�s��(1-%d):", numEmployees);
			scanf("%d", &i);
			modifySchedule(&employees[i - 1]);
			calculatePay(&employees[i - 1]); // ���s�p���~��
			break;
		case 3:
			// �C�L���u�~��
			for (int i = 0; i < numEmployees; ++i) {
				displayPayroll(&employees[i]);
			}
			break;
		case 4:
			// �����{��
			break;
		default:
			printf("�п�J���Ī��ﶵ�C\n");
		}
	} while (option != 4);

	free(employees);

	return 0;
}
