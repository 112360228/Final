#include <stdio.h>
#include <string.h>
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

//分辨班表輸入是否正確

int inputSchedule(int day, int x) {
	char input[10];
	int i = 0, output = 0;
	if (x == 0)
		printf("星期 %d: ", day + 1);
	else
		printf("請重新輸入: ");
	scanf("%s", &input);

	if (strcmp(input, "XXX") == 0) {
		if (day > 0) {/*
			day -= 1;
			printf("星期 %d: ", day + 1);
			scanf("%s", &input);
			*/
			return -1;
		}
		else {
			printf("今天是星期一 \n ");
		}
	}

	while (input[i] != '\0')
	{
		if (input[i] >= 48 && input[i] <= 49)
		{
			output *= 10;
			output += input[i] - 47;
		}
		i++;
	}
	if (output < 100)
		return inputSchedule(day, 1);
	else
		return output;
}

// 輸入員工排班情況

void inputEmployeeSchedule(Employee *employee) {
	int i, output;
	char input[10];

	printf("輸入 %s 的一周排班情況（早中晚，1表示上班，0表示不上班）:\n", employee->name);

	for (int day = 0; day < DAYS_IN_WEEK; ++day) {

		output = inputSchedule(day, 0);
		if (output == -1) {
			day -= 2; continue;
		}
		for (int i = 2; i >= 0; i--) {
			employee->schedule[day][i] = output % 10 - 1;
			output /= 10;
		}
	}
}



// 計算薪資

void calculatePay(Employee *employee) {
	int totalWorkHours = 0;
	int weekendWorkHours = 0;
	int overtimeHours = 0;

	for (int day = 0; day < DAYS_IN_WEEK; ++day) {
		int dailyWorkHours = 0; // 用於記錄當天工作的時數
		for (int slot = 0; slot < TIME_SLOTS; ++slot) {
			if (employee->schedule[day][slot] == 1) {
				dailyWorkHours += 4; // 每個時段為4小時
			}
		}
		if (day == 5 || day == 6) { // 如果是星期六或星期日
			weekendWorkHours += dailyWorkHours; // 累計周末工作時數
		}
		if (dailyWorkHours > 8) {
			overtimeHours += dailyWorkHours - 8; // 計算單日超過8小時的加班時數
		}
		totalWorkHours += dailyWorkHours; // 累計總工作時數
	}
	int regularHours = (totalWorkHours > 40) ? 40 : totalWorkHours; // 標準工時
	int holidayHours = 0; // 假日工時

	if (weekendWorkHours > 8) { // 如果周末工時超過8小時
		holidayHours = weekendWorkHours - 8; // 計算假日工時
	}

	int overtimePay = overtimeHours * HOURLY_RATE * 1.34; // 超過標準工時的時數按照1.34倍時薪計算

	employee->workHours = regularHours;
	employee->overtimeHours = overtimeHours;
	employee->holidayPay = holidayHours * HOURLY_RATE;
	employee->overtimePay = overtimePay;
	employee->totalPay = (regularHours * HOURLY_RATE) + employee->overtimePay + employee->holidayPay;
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

void displaySchedule(Employee *employees, int numEmployees) {
	printf("%-8s%-8s%-8s%-8s%-8s%-8s%-8s%-8s\n", "", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六", "星期日");
	printf("---------------------------------------------------------------\n");
	char shiftNames[TIME_SLOTS][10] = { "早班", "午班", "晚班" };
	for (int shift = 0; shift < TIME_SLOTS; shift++) {
		printf("%s\n", shiftNames[shift]);
		for (int i = 0; i < numEmployees; ++i) {
			printf("%-8s", employees[i].name);
			for (int day = 0; day < DAYS_IN_WEEK; ++day) {
				printf("%-8c", employees[i].schedule[day][shift] ? 'O' : 'X');
			}
			printf("\n");
		}
		printf("\n");
	}
}

// 修改班表
void modifySchedule(Employee *employee) {
	int num, day, timeSlot;

	printf("請輸入要更改時間的次數:");
	scanf("%d", &num);

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

	Employee *employees = (Employee *)calloc(numEmployees, sizeof(Employee)); // Use calloc to initialize memory to zero

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
			displaySchedule(employees, numEmployees);
			break;
		case 2:
			// 修改班表
			int i;
			printf("請輸入要修改的員工編號(1-%d):", numEmployees);
			scanf("%d", &i);
			modifySchedule(&employees[i - 1]);
			break;

		case 3:
			// 列印員工薪資
			for (int i = 0; i < numEmployees; ++i) {
				calculatePay(&employees[i]); //計算薪資
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