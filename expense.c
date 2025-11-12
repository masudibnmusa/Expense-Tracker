#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#ifdef _WIN32
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#endif

#define MAX_EXPENSES 1000
#define MAX_CATEGORY_LENGTH 50
#define MAX_DESCRIPTION_LENGTH 100
#define FILENAME "expenses.dat"

// Simple color codes for text only
#define COLOR_RESET "\033[0m"
#define COLOR_RED "\033[91m"
#define COLOR_GREEN "\033[92m"
#define COLOR_YELLOW "\033[93m"
#define COLOR_BLUE "\033[94m"
#define COLOR_MAGENTA "\033[95m"
#define COLOR_CYAN "\033[96m"
#define COLOR_WHITE "\033[97m"

typedef struct {
    int id;
    char date[11];  // YYYY-MM-DD
    float amount;
    char category[MAX_CATEGORY_LENGTH];
    char description[MAX_DESCRIPTION_LENGTH];
} Expense;

Expense expenses[MAX_EXPENSES];
int expense_count = 0;

// Function prototypes
void enable_colors();
void clear_screen();
void pause_screen();
void display_menu();
void add_expense();
void view_all_expenses();
void view_by_category();
void view_by_date_range();
void search_expenses();
void delete_expense();
void modify_expense();
void show_statistics();
void save_to_file();
void load_from_file();
int validate_date(const char *date);
void get_current_date(char *buffer);
void clear_input_buffer();

// Display functions
void print_success(const char *text);
void print_error(const char *text);
void print_warning(const char *text);
void print_info(const char *text);

int main() {
    int choice;
    
    enable_colors();
    clear_screen();
    
    // Welcome banner
    printf("==============================================================================\n");
    printf("                                                                              \n");
    printf("          %sEXPENSE TRACKER - FINANCIAL MANAGER%s                         \n", COLOR_CYAN, COLOR_RESET);
    printf("                                                                              \n");
    printf("==============================================================================\n\n");
    
    load_from_file();
    pause_screen();
    clear_screen();
    
    while (1) {
        display_menu();
        printf("%sEnter your choice (1-9):%s ", COLOR_CYAN, COLOR_RESET);
        
        if (scanf("%d", &choice) != 1) {
            print_error("Invalid input! Please enter a number.");
            clear_input_buffer();
            continue;
        }
        
        switch (choice) {
            case 1:
                clear_screen();
                add_expense();
                pause_screen();
                clear_screen();
                break;
            case 2:
                clear_screen();
                view_all_expenses();
                pause_screen();
                clear_screen();
                break;
            case 3:
                clear_screen();
                view_by_category();
                pause_screen();
                clear_screen();
                break;
            case 4:
                clear_screen();
                view_by_date_range();
                pause_screen();
                clear_screen();
                break;
            case 5:
                clear_screen();
                search_expenses();
                pause_screen();
                clear_screen();
                break;
            case 6:
                clear_screen();
                modify_expense();
                pause_screen();
                clear_screen();
                break;
            case 7:
                clear_screen();
                delete_expense();
                pause_screen();
                clear_screen();
                break;
            case 8:
                clear_screen();
                show_statistics();
                pause_screen();
                clear_screen();
                break;
            case 9:
                clear_screen();
                save_to_file();
                printf("\n==============================================================================\n");
                printf("                                                                              \n");
                printf("     %sThank you for using Expense Tracker! Goodbye!%s                    \n", COLOR_GREEN, COLOR_RESET);
                printf("                                                                              \n");
                printf("==============================================================================\n\n");
                exit(0);
            default:
                print_error("Invalid choice! Please enter a number between 1-9.");
        }
    }
    
    return 0;
}

void display_menu() {
    printf("\n==============================================================================\n");
    printf("              %sEXPENSE TRACKER - MAIN MENU%s                            \n", COLOR_CYAN, COLOR_RESET);
    printf("==============================================================================\n");
    printf("                                                                              \n");
    printf("  %s1.%s  Add New Expense                                                    \n", COLOR_GREEN, COLOR_RESET);
    printf("  %s2.%s  View All Expenses                                                  \n", COLOR_GREEN, COLOR_RESET);
    printf("  %s3.%s  View Expenses by Category                                         \n", COLOR_GREEN, COLOR_RESET);
    printf("  %s4.%s  View Expenses by Date Range                                       \n", COLOR_GREEN, COLOR_RESET);
    printf("  %s5.%s  Search Expenses                                                    \n", COLOR_GREEN, COLOR_RESET);
    printf("  %s6.%s  Modify Expense                                                     \n", COLOR_YELLOW, COLOR_RESET);
    printf("  %s7.%s  Delete Expense                                                     \n", COLOR_RED, COLOR_RESET);
    printf("  %s8.%s  Show Statistics                                                    \n", COLOR_BLUE, COLOR_RESET);
    printf("  %s9.%s  Exit                                                               \n", COLOR_MAGENTA, COLOR_RESET);
    printf("                                                                              \n");
    printf("==============================================================================\n");
}

void add_expense() {
    if (expense_count >= MAX_EXPENSES) {
        print_error("Expense tracker is full! Cannot add more expenses.");
        return;
    }
    
    Expense new_expense;
    new_expense.id = expense_count + 1;
    
    // Get date
    while (1) {
        printf("%sEnter date (YYYY-MM-DD) or 'today' for current date:%s ", COLOR_CYAN, COLOR_RESET);
        char date_input[20];
        scanf("%19s", date_input);
        
        if (strcmp(date_input, "today") == 0) {
            get_current_date(new_expense.date);
            break;
        } else if (validate_date(date_input)) {
            strcpy(new_expense.date, date_input);
            break;
        } else {
            print_error("Invalid date format! Please use YYYY-MM-DD format.");
        }
    }
    
    // Get amount
    while (1) {
        printf("%sEnter amount:%s ", COLOR_CYAN, COLOR_RESET);
        if (scanf("%f", &new_expense.amount) == 1 && new_expense.amount > 0) {
            break;
        } else {
            print_error("Invalid amount! Please enter a positive number.");
            clear_input_buffer();
        }
    }
    
    // Get category
    printf("%sEnter category:%s ", COLOR_CYAN, COLOR_RESET);
    clear_input_buffer();
    fgets(new_expense.category, MAX_CATEGORY_LENGTH, stdin);
    new_expense.category[strcspn(new_expense.category, "\n")] = 0;
    
    // Get description
    printf("%sEnter description:%s ", COLOR_CYAN, COLOR_RESET);
    fgets(new_expense.description, MAX_DESCRIPTION_LENGTH, stdin);
    new_expense.description[strcspn(new_expense.description, "\n")] = 0;
    
    expenses[expense_count] = new_expense;
    expense_count++;
    
    printf("\n==============================================================================\n");
    printf("  %sSUCCESS! Expense added successfully!%s\n", COLOR_GREEN, COLOR_RESET);
    printf("  %sExpense ID: %d%s\n", COLOR_YELLOW, new_expense.id, COLOR_RESET);
    printf("==============================================================================\n");
}

void view_all_expenses() {
    if (expense_count == 0) {
        print_warning("No expenses recorded.");
        return;
    }
    
    printf("\n==================================================================================\n");
    printf("                           %sALL EXPENSES%s\n", COLOR_CYAN, COLOR_RESET);
    printf("==================================================================================\n");
    printf("%-5s %-12s %-15s %-20s %-30s\n", "ID", "Date", "Amount", "Category", "Description");
    printf("----------------------------------------------------------------------------------\n");
    
    float total = 0;
    for (int i = 0; i < expense_count; i++) {
        printf("%-5d %-12s %-15.2f %-20s %-30s\n",
               expenses[i].id,
               expenses[i].date,
               expenses[i].amount,
               expenses[i].category,
               expenses[i].description);
        total += expenses[i].amount;
    }
    printf("==================================================================================\n");
    printf("  %sTOTAL EXPENSES: TK %.2f%s\n", COLOR_GREEN, total, COLOR_RESET);
    printf("==================================================================================\n");
}

void view_by_category() {
    if (expense_count == 0) {
        print_warning("No expenses recorded.");
        return;
    }
    
    char category[MAX_CATEGORY_LENGTH];
    printf("%sEnter category to view:%s ", COLOR_CYAN, COLOR_RESET);
    clear_input_buffer();
    fgets(category, MAX_CATEGORY_LENGTH, stdin);
    category[strcspn(category, "\n")] = 0;
    
    printf("\n==============================================================================\n");
    printf("           %sExpenses in Category: '%s'%s\n", COLOR_CYAN, category, COLOR_RESET);
    printf("==============================================================================\n");
    printf("%-5s %-12s %-15s %-30s\n", "ID", "Date", "Amount", "Description");
    printf("------------------------------------------------------------------------------\n");
    
    float category_total = 0;
    int found = 0;
    
    for (int i = 0; i < expense_count; i++) {
        if (strcasecmp(expenses[i].category, category) == 0) {
            printf("%-5d %-12s %-15.2f %-30s\n",
                   expenses[i].id,
                   expenses[i].date,
                   expenses[i].amount,
                   expenses[i].description);
            category_total += expenses[i].amount;
            found = 1;
        }
    }
    
    if (found) {
        printf("==============================================================================\n");
        printf("  %sCATEGORY TOTAL: TK %.2f%s\n", COLOR_GREEN, category_total, COLOR_RESET);
        printf("==============================================================================\n");
    } else {
        printf("==============================================================================\n");
        printf("  %sWARNING: No expenses found in category '%s'%s\n", COLOR_YELLOW, category, COLOR_RESET);
        printf("==============================================================================\n");
    }
}

void view_by_date_range() {
    if (expense_count == 0) {
        print_warning("No expenses recorded.");
        return;
    }
    
    char start_date[11], end_date[11];
    
    printf("%sEnter start date (YYYY-MM-DD):%s ", COLOR_CYAN, COLOR_RESET);
    scanf("%10s", start_date);
    if (!validate_date(start_date)) {
        print_error("Invalid start date format!");
        return;
    }
    
    printf("%sEnter end date (YYYY-MM-DD):%s ", COLOR_CYAN, COLOR_RESET);
    scanf("%10s", end_date);
    if (!validate_date(end_date)) {
        print_error("Invalid end date format!");
        return;
    }
    
    printf("\n==================================================================================\n");
    printf("       %sExpenses from %s to %s%s\n", COLOR_CYAN, start_date, end_date, COLOR_RESET);
    printf("==================================================================================\n");
    printf("%-5s %-12s %-15s %-20s %-30s\n", "ID", "Date", "Amount", "Category", "Description");
    printf("----------------------------------------------------------------------------------\n");
    
    float period_total = 0;
    int found = 0;
    
    for (int i = 0; i < expense_count; i++) {
        if (strcmp(expenses[i].date, start_date) >= 0 && 
            strcmp(expenses[i].date, end_date) <= 0) {
            printf("%-5d %-12s %-15.2f %-20s %-30s\n",
                   expenses[i].id,
                   expenses[i].date,
                   expenses[i].amount,
                   expenses[i].category,
                   expenses[i].description);
            period_total += expenses[i].amount;
            found = 1;
        }
    }
    
    if (found) {
        printf("==================================================================================\n");
        printf("  %sPERIOD TOTAL: TK %.2f%s\n", COLOR_GREEN, period_total, COLOR_RESET);
        printf("==================================================================================\n");
    } else {
        printf("==================================================================================\n");
        printf("  %sWARNING: No expenses found in the specified date range.%s\n", COLOR_YELLOW, COLOR_RESET);
        printf("==================================================================================\n");
    }
}

void search_expenses() {
    if (expense_count == 0) {
        print_warning("No expenses recorded.");
        return;
    }
    
    char search_term[MAX_DESCRIPTION_LENGTH];
    printf("%sEnter search term (description or category):%s ", COLOR_CYAN, COLOR_RESET);
    clear_input_buffer();
    fgets(search_term, MAX_DESCRIPTION_LENGTH, stdin);
    search_term[strcspn(search_term, "\n")] = 0;
    
    printf("\n==================================================================================\n");
    printf("               %sSearch Results for: '%s'%s\n", COLOR_CYAN, search_term, COLOR_RESET);
    printf("==================================================================================\n");
    printf("%-5s %-12s %-15s %-20s %-30s\n", "ID", "Date", "Amount", "Category", "Description");
    printf("----------------------------------------------------------------------------------\n");
    
    float search_total = 0;
    int found = 0;
    
    for (int i = 0; i < expense_count; i++) {
        if (strstr(expenses[i].description, search_term) != NULL ||
            strstr(expenses[i].category, search_term) != NULL) {
            printf("%-5d %-12s %-15.2f %-20s %-30s\n",
                   expenses[i].id,
                   expenses[i].date,
                   expenses[i].amount,
                   expenses[i].category,
                   expenses[i].description);
            search_total += expenses[i].amount;
            found = 1;
        }
    }
    
    if (found) {
        printf("==================================================================================\n");
        printf("  %sSEARCH TOTAL: TK %.2f%s\n", COLOR_GREEN, search_total, COLOR_RESET);
        printf("==================================================================================\n");
    } else {
        printf("==================================================================================\n");
        printf("  %sWARNING: No expenses found matching your search.%s\n", COLOR_YELLOW, COLOR_RESET);
        printf("==================================================================================\n");
    }
}

void modify_expense() {
    if (expense_count == 0) {
        print_warning("No expenses recorded.");
        return;
    }
    
    int id;
    printf("%sEnter expense ID to modify:%s ", COLOR_CYAN, COLOR_RESET);
    if (scanf("%d", &id) != 1) {
        print_error("Invalid ID!");
        clear_input_buffer();
        return;
    }
    
    int found = -1;
    for (int i = 0; i < expense_count; i++) {
        if (expenses[i].id == id) {
            found = i;
            break;
        }
    }
    
    if (found == -1) {
        print_error("Expense with specified ID not found.");
        return;
    }
    
    printf("\n%sCurrent expense details:%s\n", COLOR_CYAN, COLOR_RESET);
    printf("Date: %s\n", expenses[found].date);
    printf("Amount: TK %.2f\n", expenses[found].amount);
    printf("Category: %s\n", expenses[found].category);
    printf("Description: %s\n", expenses[found].description);
    
    printf("\n%sEnter new details (press Enter to keep current value):%s\n", COLOR_YELLOW, COLOR_RESET);
    
    // Get new date
    char new_date[20];
    printf("New date (YYYY-MM-DD) [%s]: ", expenses[found].date);
    clear_input_buffer();
    fgets(new_date, sizeof(new_date), stdin);
    new_date[strcspn(new_date, "\n")] = 0;
    if (strlen(new_date) > 0 && validate_date(new_date)) {
        strncpy(expenses[found].date, new_date, sizeof(expenses[found].date) - 1);
    }
    
    // Get new amount
    char amount_input[20];
    printf("New amount [%.2f]: ", expenses[found].amount);
    fgets(amount_input, sizeof(amount_input), stdin);
    amount_input[strcspn(amount_input, "\n")] = 0;
    if (strlen(amount_input) > 0) {
        float new_amount = atof(amount_input);
        if (new_amount > 0) {
            expenses[found].amount = new_amount;
        }
    }
    
    // Get new category
    char new_category[MAX_CATEGORY_LENGTH];
    printf("New category [%s]: ", expenses[found].category);
    fgets(new_category, sizeof(new_category), stdin);
    new_category[strcspn(new_category, "\n")] = 0;
    if (strlen(new_category) > 0) {
        strncpy(expenses[found].category, new_category, sizeof(expenses[found].category) - 1);
    }
    
    // Get new description
    char new_description[MAX_DESCRIPTION_LENGTH];
    printf("New description [%s]: ", expenses[found].description);
    fgets(new_description, sizeof(new_description), stdin);
    new_description[strcspn(new_description, "\n")] = 0;
    if (strlen(new_description) > 0) {
        strncpy(expenses[found].description, new_description, sizeof(expenses[found].description) - 1);
    }
    
    print_success("Expense modified successfully!");
}

void delete_expense() {
    if (expense_count == 0) {
        print_warning("No expenses recorded.");
        return;
    }
    
    int id;
    printf("%sEnter expense ID to delete:%s ", COLOR_CYAN, COLOR_RESET);
    if (scanf("%d", &id) != 1) {
        print_error("Invalid ID!");
        clear_input_buffer();
        return;
    }
    
    int found = -1;
    for (int i = 0; i < expense_count; i++) {
        if (expenses[i].id == id) {
            found = i;
            break;
        }
    }
    
    if (found == -1) {
        print_error("Expense with specified ID not found.");
        return;
    }
    
    printf("\n%sExpense to delete:%s\n", COLOR_RED, COLOR_RESET);
    printf("ID: %d\n", expenses[found].id);
    printf("Date: %s\n", expenses[found].date);
    printf("Amount: TK %.2f\n", expenses[found].amount);
    printf("Category: %s\n", expenses[found].category);
    printf("Description: %s\n", expenses[found].description);
    
    printf("\n%sAre you sure you want to delete this expense? (y/n):%s ", COLOR_RED, COLOR_RESET);
    clear_input_buffer();
    char confirm;
    scanf("%c", &confirm);
    
    if (confirm == 'y' || confirm == 'Y') {
        // Shift all elements after the found expense
        for (int i = found; i < expense_count - 1; i++) {
            expenses[i] = expenses[i + 1];
        }
        expense_count--;
        print_success("Expense deleted successfully!");
    } else {
        print_warning("Deletion cancelled.");
    }
}

void show_statistics() {
    if (expense_count == 0) {
        print_warning("No expenses recorded.");
        return;
    }
    
    printf("\n==================================================================================\n");
    printf("                      %sEXPENSE STATISTICS DASHBOARD%s\n", COLOR_BLUE, COLOR_RESET);
    printf("==================================================================================\n");
    
    // Total expenses
    float total = 0;
    for (int i = 0; i < expense_count; i++) {
        total += expenses[i].amount;
    }
    
    printf("\n");
    printf("  %sTotal Expenses:%s    TK %.2f\n", COLOR_CYAN, COLOR_RESET, total);
    printf("  %sAverage Expense:%s   TK %.2f\n", COLOR_CYAN, COLOR_RESET, total / expense_count);
    printf("  %sTotal Entries:%s     %d\n", COLOR_CYAN, COLOR_RESET, expense_count);
    printf("\n");
    printf("==================================================================================\n");
    printf("                      %sCATEGORY BREAKDOWN%s\n", COLOR_MAGENTA, COLOR_RESET);
    printf("==================================================================================\n");
    
    // Find unique categories and their totals
    char categories[MAX_EXPENSES][MAX_CATEGORY_LENGTH];
    float category_totals[MAX_EXPENSES] = {0};
    int category_count = 0;
    
    for (int i = 0; i < expense_count; i++) {
        int found = 0;
        for (int j = 0; j < category_count; j++) {
            if (strcasecmp(categories[j], expenses[i].category) == 0) {
                category_totals[j] += expenses[i].amount;
                found = 1;
                break;
            }
        }
        if (!found) {
            strcpy(categories[category_count], expenses[i].category);
            category_totals[category_count] = expenses[i].amount;
            category_count++;
        }
    }
    
    // Display category breakdown
    for (int i = 0; i < category_count; i++) {
        float percentage = (category_totals[i] / total) * 100;
        printf("  %-20s: TK %-10.2f (%5.1f%%)\n", 
               categories[i], category_totals[i], percentage);
    }
    
    // Find highest and lowest expense
    Expense highest = expenses[0];
    Expense lowest = expenses[0];
    for (int i = 1; i < expense_count; i++) {
        if (expenses[i].amount > highest.amount) {
            highest = expenses[i];
        }
        if (expenses[i].amount < lowest.amount) {
            lowest = expenses[i];
        }
    }
    
    printf("\n");
    printf("==================================================================================\n");
    printf("  %sHighest Expense:%s TK %.2f\n", COLOR_RED, COLOR_RESET, highest.amount);
    printf("     Category: %s, Description: %s\n", highest.category, highest.description);
    printf("\n");
    printf("  %sLowest Expense:%s  TK %.2f\n", COLOR_GREEN, COLOR_RESET, lowest.amount);
    printf("     Category: %s, Description: %s\n", lowest.category, lowest.description);
    printf("==================================================================================\n");
}

void save_to_file() {
    FILE *file = fopen(FILENAME, "wb");
    if (file == NULL) {
        print_error("Could not save data to file!");
        return;
    }
    
    // Save expense count first
    if (fwrite(&expense_count, sizeof(int), 1, file) != 1) {
        print_error("Error writing expense count!");
        fclose(file);
        return;
    }
    
    // Save all expenses
    for (int i = 0; i < expense_count; i++) {
        if (fwrite(&expenses[i], sizeof(Expense), 1, file) != 1) {
            print_error("Error writing expense data!");
            fclose(file);
            return;
        }
    }
    
    fclose(file);
    print_success("Data saved successfully!");
}

void load_from_file() {
    FILE *file = fopen(FILENAME, "rb");
    if (file == NULL) {
        print_warning("No previous data found. Starting fresh.");
        return;
    }
    
    // Read expense count
    if (fread(&expense_count, sizeof(int), 1, file) != 1) {
        print_warning("Error reading data file. Starting fresh.");
        fclose(file);
        expense_count = 0;
        return;
    }
    
    // Read all expenses
    for (int i = 0; i < expense_count; i++) {
        if (fread(&expenses[i], sizeof(Expense), 1, file) != 1) {
            print_error("Error reading expense data!");
            fclose(file);
            expense_count = i;
            return;
        }
    }
    
    fclose(file);
    printf("%sData loaded successfully! (%d expenses)%s\n", COLOR_GREEN, expense_count, COLOR_RESET);
}

int validate_date(const char *date) {
    if (strlen(date) != 10) return 0;
    if (date[4] != '-' || date[7] != '-') return 0;
    
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (!isdigit(date[i])) return 0;
    }
    
    return 1;
}

void get_current_date(char *buffer) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(buffer, 11, "%Y-%m-%d", tm_info);
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pause_screen() {
    printf("\n%sPress Enter to continue...%s", COLOR_CYAN, COLOR_RESET);
    clear_input_buffer();
    getchar();
}

void enable_colors() {
    #ifdef _WIN32
    // Enable ANSI color codes in Windows 10+
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode;
    if (GetConsoleMode(hConsole, &mode)) {
        SetConsoleMode(hConsole, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
    #endif
}

// Display functions
void print_success(const char *text) {
    printf("%s✓ %s%s\n", COLOR_GREEN, text, COLOR_RESET);
}

void print_error(const char *text) {
    printf("%s✗ %s%s\n", COLOR_RED, text, COLOR_RESET);
}

void print_warning(const char *text) {
    printf("%s⚠ %s%s\n", COLOR_YELLOW, text, COLOR_RESET);
}

void print_info(const char *text) {
    printf("%sℹ %s%s\n", COLOR_CYAN, text, COLOR_RESET);
}
