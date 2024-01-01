#include <stdio.h>
#include <stdlib.h>

#define DAYS_IN_WEEK 7 // 一周的天數
#define TIME_SLOTS 3   // 早中晚三個時段
#define HOURLY_RATE 180 // 正常時薪

typedef struct {
	char name[50];
	int workHours;                  // 工作時數
	int schedule[DAYS_IN_WEEK][TIME_SLOTS]; // 一周的排班表
	int overtimeHours;              // 加班時數
	int overtimePay;                // 加班費用
	int holidayPay;                 // 假日薪資
	int totalPay;                   // 整周薪水
} Employee;

// 輸入員工排班情況
void inputEmployeeSchedule(Employee *employee) {
	printf("輸入 %s 的一周排班情況（早中晚，1表示上班，0表示不上班）:\n", employee->name);
	for (int day = 0; day < DAYS_IN_WEEK; ++day) {
		printf("禮拜 %d: ", day + 1);
		scanf("%d %d %d", &employee->schedule[day][0], &employee->schedule[day][1], &employee->schedule[day][2]);
	}
}

// 計算薪資
void calculatePay(Employee *employee) {
	int totalWorkHours = 0;

	// 計算總工作時數
	for (int day = 0; day < DAYS_IN_WEEK; ++day) {
		for (int slot = 0; slot < TIME_SLOTS; ++slot) {
			if (employee->schedule[day][slot] == 1) {
				totalWorkHours += 4; // 一個時間段是4小時
			}
		}
	}

	// 正常工作時數不得超過40小時
	if (totalWorkHours > 40) {
		employee->overtimeHours = totalWorkHours - 40;
		employee->workHours = 40;
		employee->overtimePay = employee->overtimeHours * HOURLY_RATE * 2; // 加班費用是原本的兩倍
	}
	else {
		employee->overtimeHours = 0;
		employee->workHours = totalWorkHours;
		employee->overtimePay = 0;
	}

	// 計算假日薪資
	employee->holidayPay = totalWorkHours * HOURLY_RATE;

	// 計算整周薪水
	employee->totalPay = (employee->workHours * HOURLY_RATE) + employee->overtimePay + employee->holidayPay;
}

// 顯示薪資明細
void displayPayroll(Employee *employee) {
	printf("\n員工姓名：%s\n", employee->name);
	printf("正常工時：%d 小時\n", employee->workHours);
	printf("加班時數：%d 小時\n", employee->overtimeHours);
	printf("加班費用：%d 元\n", employee->overtimePay);
	printf("假日薪資：%d 元\n", employee->holidayPay);
	printf("整周薪水：%d 元\n", employee->totalPay);
}

// 列印班表
void printSchedule(Employee *employee) {
	printf("\n員工 %s 的一周排班情況:\n", employee->name);
	for (int day = 0; day < DAYS_IN_WEEK; ++day) {
		printf("星期 %d: %d %d %d\n", day + 1, employee->schedule[day][0], employee->schedule[day][1], employee->schedule[day][2]);
	}
}

// 修改班表
void modifySchedule(Employee *employee) {
	int num,day, timeSlot;

	printf("請輸入要更改時間的次數:");
	scanf("%d",&num);
	for (int i = 0; i < num; i++) {
		printf("\n請輸入要修改員工 %s 的排班的日期(1-7)和時段(1-3)：", employee->name);
		scanf("%d %d", &day, &timeSlot);
		printf("輸入新的排班情況（1表示上班，0表示不上班）：");
		scanf("%d", &employee->schedule[day - 1][timeSlot - 1]);
	}
}

int main() {
	int numEmployees;
	int option;

	printf("請輸入員工人數：");
	scanf("%d", &numEmployees);

	Employee *employees = (Employee *)malloc(numEmployees * sizeof(Employee));

	// 輸入員工名字和排班情況
	for (int i = 0; i < numEmployees; ++i) {
		printf("輸入第 %d 位員工的姓名: ", i + 1);
		scanf("%s", employees[i].name);
		inputEmployeeSchedule(&employees[i]);
	}

	// 功能選單
	do {
		printf("\n選擇功能：\n");
		printf("1. 列印班表\n");
		printf("2. 修改班表\n");
		printf("3. 列印員工薪資\n");
		printf("4. 結束程式\n");
		scanf("%d", &option);

		switch (option) {
		case 1:
			// 列印班表
			for (int i = 0; i < numEmployees; ++i) {
				printSchedule(&employees[i]);
			}
			break;
		case 2:
			// 修改班表
			int i;
			printf("請輸入要修改的員工編號(1-%d):",numEmployees);
			scanf("%d", &i);
			modifySchedule(&employees[i-1]);
			calculatePay(&employees[i-1]); // 重新計算薪資
			break;
		case 3:
			// 列印員工薪資
			for (int i = 0; i < numEmployees; ++i) {
				displayPayroll(&employees[i]);
			}
			break;
		case 4:
			// 結束程式
			break;
		default:
			printf("請輸入有效的選項。\n");
		}
	} while (option != 4);

	free(employees);

	return 0;
}