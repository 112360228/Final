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

int main() {
	int numEmployees;

	printf("請輸入員工人數：");
	scanf("%d", &numEmployees);

	Employee *employees = (Employee *)malloc(numEmployees * sizeof(Employee));

	// 輸入員工名字和排班情況
	for (int i = 0; i < numEmployees; ++i) {
		printf("輸入第 %d 位員工的姓名: ", i + 1);
		scanf("%s", employees[i].name);
		inputEmployeeSchedule(&employees[i]);
	}

	// 計算薪資並顯示薪資明細
	for (int i = 0; i < numEmployees; ++i) {
		calculatePay(&employees[i]);
		displayPayroll(&employees[i]);
	}

	free(employees);

	return 0;
}
