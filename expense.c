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

// Color codes
#define COLOR_RESET "\033[0m"
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN "\033[36m"
#define COLOR_WHITE "\033[37m"
#define COLOR_BRIGHT_RED "\033[91m"
#define COLOR_BRIGHT_GREEN "\033[92m"
#define COLOR_BRIGHT_YELLOW "\033[93m"
#define COLOR_BRIGHT_BLUE "\033[94m"
#define COLOR_BRIGHT_MAGENTA "\033[95m"
#define COLOR_BRIGHT_CYAN "\033[96m"
#define COLOR_BOLD "\033[1m"

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

int main() {
    int choice;
    
    enable_colors();
    clear_screen();
    
    // Welcome banner
    printf("%s\n", COLOR_BRIGHT_CYAN);
    printf("===============================================================\n");
    printf("                                                               \n");
    printf("          %s%sEXPENSE TRACKER - FINANCIAL MANAGER%s%s          \n", COLOR_BOLD, COLOR_BRIGHT_YELLOW, COLOR_RESET, COLOR_BRIGHT_CYAN);
    printf("                                                               \n");
    printf("===============================================================%s\n\n", COLOR_RESET);
    
    load_from_file();
    
    while (1) {
        display_menu();
        printf("Enter your choice (1-9): ");
        
        if (scanf("%d", &choice) != 1) {
            printf("%sInvalid input! Please enter a number.%s\n", COLOR_RED, COLOR_RESET);
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
                printf("\n%s", COLOR_BRIGHT_CYAN);
                printf("===============================================================\n");
                printf("                                                               \n");
                printf("     %s%sThank you for using Expense Tracker! Goodbye!%s%s      \n", COLOR_BOLD, COLOR_BRIGHT_GREEN, COLOR_RESET, COLOR_BRIGHT_CYAN);
                printf("                                                               \n");
                printf("===============================================================%s\n\n", COLOR_RESET);
                exit(0);
            default:
                printf("%sInvalid choice! Please enter a number between 1-9.%s\n", COLOR_RED, COLOR_RESET);
        }
    }
    
    return 0;
}

void display_menu() {
    printf("\n%s", COLOR_CYAN);
    printf("===============================================================\n");
    printf("              %s%sEXPENSE TRACKER - MAIN MENU%s%s              \n", COLOR_BOLD, COLOR_BRIGHT_CYAN, COLOR_RESET, COLOR_CYAN);
    printf("===============================================================%s\n", COLOR_RESET);
    printf("                                                               \n");
    printf("  %s1.%s  Add New Expense                                       \n", COLOR_BRIGHT_GREEN, COLOR_RESET);
    printf("  %s2.%s  View All Expenses                                     \n", COLOR_BRIGHT_GREEN, COLOR_RESET);
    printf("  %s3.%s  View Expenses by Category                            \n", COLOR_BRIGHT_GREEN, COLOR_RESET);
    printf("  %s4.%s  View Expenses by Date Range                          \n", COLOR_BRIGHT_GREEN, COLOR_RESET);
    printf("  %s5.%s  Search Expenses                                       \n", COLOR_BRIGHT_GREEN, COLOR_RESET);
    printf("  %s6.%s  Modify Expense                                        \n", COLOR_BRIGHT_YELLOW, COLOR_RESET);
    printf("  %s7.%s  Delete Expense                                        \n", COLOR_BRIGHT_RED, COLOR_RESET);
    printf("  %s8.%s  Show Statistics                                       \n", COLOR_BRIGHT_BLUE, COLOR_RESET);
    printf("  %s9.%s  Exit                                                  \n", COLOR_BRIGHT_MAGENTA, COLOR_RESET);
    printf("                                                               \n");
    printf("%s===============================================================%s\n", COLOR_CYAN, COLOR_RESET);
}

void add_expense() {
    if (expense_count >= MAX_EXPENSES) {
        printf("%sError: Expense tracker is full! Cannot add more expenses.%s\n", COLOR_RED, COLOR_RESET);
        return;
    }
    
    Expense new_expense;
    new_expense.id = expense_count + 1;
    
    // Get date
    while (1) {
        printf("Enter date (YYYY-MM-DD) or 'today' for current date: ");
        char date_input[20];
        scanf("%s", date_input);
        
        if (strcmp(date_input, "today") == 0) {
            get_current_date(new_expense.date);
            break;
        } else if (validate_date(date_input)) {
            strcpy(new_expense.date, date_input);
            break;
        } else {
            printf("%sInvalid date format! Please use YYYY-MM-DD format.%s\n", COLOR_RED, COLOR_RESET);
        }
    }
    
    // Get amount
    while (1) {
        printf("Enter amount: ");
        if (scanf("%f", &new_expense.amount) == 1 && new_expense.amount > 0) {
            break;
        } else {
            printf("%sInvalid amount! Please enter a positive number.%s\n", COLOR_RED, COLOR_RESET);
            clear_input_buffer();
        }
    }
    
    // Get category
    printf("Enter category: ");
    clear_input_buffer();
    fgets(new_expense.category, MAX_CATEGORY_LENGTH, stdin);
    new_expense.category[strcspn(new_expense.category, "\n")] = 0;
    
    // Get description
    printf("Enter description: ");
    fgets(new_expense.description, MAX_DESCRIPTION_LENGTH, stdin);
    new_expense.description[strcspn(new_expense.description, "\n")] = 0;
    
    expenses[expense_count] = new_expense;
    expense_count++;
    
    printf("\n%s", COLOR_GREEN);
    printf("===========================================================\n");
    printf("  %sSUCCESS! Expense added successfully!%s                 \n", COLOR_BRIGHT_GREEN, COLOR_RESET);
    printf("  %sExpense ID: %d%s                                       \n", COLOR_BRIGHT_YELLOW, new_expense.id, COLOR_RESET);
    printf("===========================================================%s\n", COLOR_RESET);
}

void view_all_expenses() {
    if (expense_count == 0) {
        printf("\n%sWARNING: No expenses recorded.%s\n", COLOR_YELLOW, COLOR_RESET);
        return;
    }
    
    printf("\n%s", COLOR_CYAN);
    printf("===================================================================================\n");
    printf("                           %s%sALL EXPENSES%s%s                                       \n", COLOR_BOLD, COLOR_BRIGHT_CYAN, COLOR_RESET, COLOR_CYAN);
    printf("===================================================================================%s\n", COLOR_RESET);
    printf("%s%s%-5s %-12s %-15s %-20s %-30s%s\n", COLOR_BOLD, COLOR_CYAN,
           "ID", "Date", "Amount", "Category", "Description", COLOR_RESET);
    printf("%s-----------------------------------------------------------------------------------%s\n", COLOR_CYAN, COLOR_RESET);
    
    float total = 0;
    for (int i = 0; i < expense_count; i++) {
        printf("%-5d %-12s %sTK %-12.2f%s %-20s %-30s\n",
               expenses[i].id,
               expenses[i].date,
               COLOR_BRIGHT_GREEN, expenses[i].amount, COLOR_RESET,
               expenses[i].category,
               expenses[i].description);
        total += expenses[i].amount;
    }
    printf("%s===================================================================================%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s  %s%sTOTAL EXPENSES:%s%s %sTK %.2f%s                                                  %s\n", 
           COLOR_CYAN, COLOR_BOLD, COLOR_BRIGHT_YELLOW, COLOR_RESET, COLOR_CYAN,
           COLOR_BRIGHT_GREEN, total, COLOR_RESET, COLOR_CYAN);
    printf("===================================================================================%s\n", COLOR_RESET);
}

void view_by_category() {
    if (expense_count == 0) {
        printf("%sNo expenses recorded.%s\n", COLOR_YELLOW, COLOR_RESET);
        return;
    }
    
    char category[MAX_CATEGORY_LENGTH];
    printf("Enter category to view: ");
    clear_input_buffer();
    fgets(category, MAX_CATEGORY_LENGTH, stdin);
    category[strcspn(category, "\n")] = 0;
    
    printf("\n%s", COLOR_CYAN);
    printf("===========================================================================\n");
    printf("           %s%sExpenses in Category: '%s'%s%s                 \n", COLOR_BOLD, COLOR_BRIGHT_CYAN, category, COLOR_RESET, COLOR_CYAN);
    printf("===========================================================================%s\n", COLOR_RESET);
    printf("%s%s%-5s %-12s %-15s %-30s%s\n", COLOR_BOLD, COLOR_CYAN, "ID", "Date", "Amount", "Description", COLOR_RESET);
    printf("%s---------------------------------------------------------------------------%s\n", COLOR_CYAN, COLOR_RESET);
    
    float category_total = 0;
    int found = 0;
    
    for (int i = 0; i < expense_count; i++) {
        if (strcasecmp(expenses[i].category, category) == 0) {
            printf("%-5d %-12s %sTK %-12.2f%s %-30s\n",
                   expenses[i].id,
                   expenses[i].date,
                   COLOR_BRIGHT_GREEN, expenses[i].amount, COLOR_RESET,
                   expenses[i].description);
            category_total += expenses[i].amount;
            found = 1;
        }
    }
    
    if (found) {
        printf("%s===========================================================================%s\n", COLOR_CYAN, COLOR_RESET);
        printf("%s  %s%sCATEGORY TOTAL:%s%s %sTK %.2f%s                                        %s\n", 
               COLOR_CYAN, COLOR_BOLD, COLOR_BRIGHT_YELLOW, COLOR_RESET, COLOR_CYAN,
               COLOR_BRIGHT_GREEN, category_total, COLOR_RESET, COLOR_CYAN);
        printf("===========================================================================%s\n", COLOR_RESET);
    } else {
        printf("%s===========================================================================%s\n", COLOR_CYAN, COLOR_RESET);
        printf("%s  %sWARNING: No expenses found in category '%s'%s                        %s\n", COLOR_CYAN, COLOR_YELLOW, category, COLOR_RESET, COLOR_CYAN);
        printf("===========================================================================%s\n", COLOR_RESET);
    }
}

void view_by_date_range() {
    if (expense_count == 0) {
        printf("%sNo expenses recorded.%s\n", COLOR_YELLOW, COLOR_RESET);
        return;
    }
    
    char start_date[11], end_date[11];
    
    printf("Enter start date (YYYY-MM-DD): ");
    scanf("%s", start_date);
    if (!validate_date(start_date)) {
        printf("%sInvalid start date format!%s\n", COLOR_RED, COLOR_RESET);
        return;
    }
    
    printf("Enter end date (YYYY-MM-DD): ");
    scanf("%s", end_date);
    if (!validate_date(end_date)) {
        printf("%sInvalid end date format!%s\n", COLOR_RED, COLOR_RESET);
        return;
    }
    
    printf("\n%s", COLOR_CYAN);
    printf("===================================================================================\n");
    printf("       %s%sExpenses from %s to %s%s%s                    \n", COLOR_BOLD, COLOR_BRIGHT_CYAN, start_date, end_date, COLOR_RESET, COLOR_CYAN);
    printf("===================================================================================%s\n", COLOR_RESET);
    printf("%s%s%-5s %-12s %-15s %-20s %-30s%s\n", COLOR_BOLD, COLOR_CYAN, "ID", "Date", "Amount", "Category", "Description", COLOR_RESET);
    printf("%s-----------------------------------------------------------------------------------%s\n", COLOR_CYAN, COLOR_RESET);
    
    float period_total = 0;
    int found = 0;
    
    for (int i = 0; i < expense_count; i++) {
        if (strcmp(expenses[i].date, start_date) >= 0 && 
            strcmp(expenses[i].date, end_date) <= 0) {
            printf("%-5d %-12s %sTK %-12.2f%s %-20s %-30s\n",
                   expenses[i].id,
                   expenses[i].date,
                   COLOR_BRIGHT_GREEN, expenses[i].amount, COLOR_RESET,
                   expenses[i].category,
                   expenses[i].description);
            period_total += expenses[i].amount;
            found = 1;
        }
    }
    
    if (found) {
        printf("%s===================================================================================%s\n", COLOR_CYAN, COLOR_RESET);
        printf("%s  %s%sPERIOD TOTAL:%s%s %sTK %.2f%s                                                    %s\n", 
               COLOR_CYAN, COLOR_BOLD, COLOR_BRIGHT_YELLOW, COLOR_RESET, COLOR_CYAN,
               COLOR_BRIGHT_GREEN, period_total, COLOR_RESET, COLOR_CYAN);
        printf("===================================================================================%s\n", COLOR_RESET);
    } else {
        printf("%s===================================================================================%s\n", COLOR_CYAN, COLOR_RESET);
        printf("%s  %sWARNING: No expenses found in the specified date range.%s                              %s\n", COLOR_CYAN, COLOR_YELLOW, COLOR_RESET, COLOR_CYAN);
        printf("===================================================================================%s\n", COLOR_RESET);
    }
}

void search_expenses() {
    if (expense_count == 0) {
        printf("%sNo expenses recorded.%s\n", COLOR_YELLOW, COLOR_RESET);
        return;
    }
    
    char search_term[MAX_DESCRIPTION_LENGTH];
    printf("Enter search term (description or category): ");
    clear_input_buffer();
    fgets(search_term, MAX_DESCRIPTION_LENGTH, stdin);
    search_term[strcspn(search_term, "\n")] = 0;
    
    printf("\n%s", COLOR_CYAN);
    printf("===================================================================================\n");
    printf("               %s%sSearch Results for: '%s'%s%s                         \n", COLOR_BOLD, COLOR_BRIGHT_CYAN, search_term, COLOR_RESET, COLOR_CYAN);
    printf("===================================================================================%s\n", COLOR_RESET);
    printf("%s%s%-5s %-12s %-15s %-20s %-30s%s\n", COLOR_BOLD, COLOR_CYAN, "ID", "Date", "Amount", "Category", "Description", COLOR_RESET);
    printf("%s-----------------------------------------------------------------------------------%s\n", COLOR_CYAN, COLOR_RESET);
    
    float search_total = 0;
    int found = 0;
    
    for (int i = 0; i < expense_count; i++) {
        if (strstr(expenses[i].description, search_term) != NULL ||
            strstr(expenses[i].category, search_term) != NULL) {
            printf("%-5d %-12s %sTK %-12.2f%s %-20s %-30s\n",
                   expenses[i].id,
                   expenses[i].date,
                   COLOR_BRIGHT_GREEN, expenses[i].amount, COLOR_RESET,
                   expenses[i].category,
                   expenses[i].description);
            search_total += expenses[i].amount;
            found = 1;
        }
    }
    
    if (found) {
        printf("%s===================================================================================%s\n", COLOR_CYAN, COLOR_RESET);
        printf("%s  %s%sSEARCH TOTAL:%s%s %sTK %.2f%s                                                     %s\n", 
               COLOR_CYAN, COLOR_BOLD, COLOR_BRIGHT_YELLOW, COLOR_RESET, COLOR_CYAN,
               COLOR_BRIGHT_GREEN, search_total, COLOR_RESET, COLOR_CYAN);
        printf("===================================================================================%s\n", COLOR_RESET);
    } else {
        printf("%s===================================================================================%s\n", COLOR_CYAN, COLOR_RESET);
        printf("%s  %sWARNING: No expenses found matching your search.%s                                     %s\n", COLOR_CYAN, COLOR_YELLOW, COLOR_RESET, COLOR_CYAN);
        printf("===================================================================================%s\n", COLOR_RESET);
    }
}

void modify_expense() {
    if (expense_count == 0) {
        printf("%sNo expenses recorded.%s\n", COLOR_YELLOW, COLOR_RESET);
        return;
    }
    
    int id;
    printf("Enter expense ID to modify: ");
    if (scanf("%d", &id) != 1) {
        printf("%sInvalid ID!%s\n", COLOR_RED, COLOR_RESET);
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
        printf("%sExpense with ID %d not found.%s\n", COLOR_RED, id, COLOR_RESET);
        return;
    }
    
    printf("\n%sCurrent expense details:%s\n", COLOR_BRIGHT_CYAN, COLOR_RESET);
    printf("Date: %s\n", expenses[found].date);
    printf("Amount: %sTK %.2f%s\n", COLOR_GREEN, expenses[found].amount, COLOR_RESET);
    printf("Category: %s\n", expenses[found].category);
    printf("Description: %s\n", expenses[found].description);
    
    printf("\nEnter new details (press Enter to keep current value):\n");
    
    // Get new date
    char new_date[11];
    printf("New date (YYYY-MM-DD) [%s]: ", expenses[found].date);
    clear_input_buffer();
    fgets(new_date, sizeof(new_date), stdin);
    new_date[strcspn(new_date, "\n")] = 0;
    if (strlen(new_date) > 0 && validate_date(new_date)) {
        strcpy(expenses[found].date, new_date);
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
        strcpy(expenses[found].category, new_category);
    }
    
    // Get new description
    char new_description[MAX_DESCRIPTION_LENGTH];
    printf("New description [%s]: ", expenses[found].description);
    fgets(new_description, sizeof(new_description), stdin);
    new_description[strcspn(new_description, "\n")] = 0;
    if (strlen(new_description) > 0) {
        strcpy(expenses[found].description, new_description);
    }
    
    printf("%sExpense modified successfully!%s\n", COLOR_BRIGHT_GREEN, COLOR_RESET);
}

void delete_expense() {
    if (expense_count == 0) {
        printf("%sNo expenses recorded.%s\n", COLOR_YELLOW, COLOR_RESET);
        return;
    }
    
    int id;
    printf("Enter expense ID to delete: ");
    if (scanf("%d", &id) != 1) {
        printf("%sInvalid ID!%s\n", COLOR_RED, COLOR_RESET);
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
        printf("%sExpense with ID %d not found.%s\n", COLOR_RED, id, COLOR_RESET);
        return;
    }
    
    printf("\n%sExpense to delete:%s\n", COLOR_BRIGHT_RED, COLOR_RESET);
    printf("ID: %d\n", expenses[found].id);
    printf("Date: %s\n", expenses[found].date);
    printf("Amount: %sTK %.2f%s\n", COLOR_GREEN, expenses[found].amount, COLOR_RESET);
    printf("Category: %s\n", expenses[found].category);
    printf("Description: %s\n", expenses[found].description);
    
    printf("\nAre you sure you want to delete this expense? (y/n): ");
    clear_input_buffer();
    char confirm;
    scanf("%c", &confirm);
    
    if (confirm == 'y' || confirm == 'Y') {
        // Shift all elements after the found expense
        for (int i = found; i < expense_count - 1; i++) {
            expenses[i] = expenses[i + 1];
        }
        expense_count--;
        printf("%sExpense deleted successfully!%s\n", COLOR_BRIGHT_GREEN, COLOR_RESET);
    } else {
        printf("%sDeletion cancelled.%s\n", COLOR_YELLOW, COLOR_RESET);
    }
}

void show_statistics() {
    if (expense_count == 0) {
        printf("\n%sWARNING: No expenses recorded.%s\n", COLOR_YELLOW, COLOR_RESET);
        return;
    }
    
    printf("\n%s", COLOR_BLUE);
    printf("===================================================================================\n");
    printf("                      %s%sEXPENSE STATISTICS DASHBOARD%s%s                        \n", COLOR_BOLD, COLOR_BRIGHT_BLUE, COLOR_RESET, COLOR_BLUE);
    printf("===================================================================================%s\n", COLOR_RESET);
    
    // Total expenses
    float total = 0;
    for (int i = 0; i < expense_count; i++) {
        total += expenses[i].amount;
    }
    printf("                                                                               \n");
    printf("%s  %sTotal Expenses:%s    %sTK %.2f%s                                              %s\n", 
           COLOR_CYAN, COLOR_BRIGHT_CYAN, COLOR_RESET, COLOR_BRIGHT_GREEN, total, COLOR_RESET, COLOR_CYAN);
    printf("%s  %sAverage Expense:%s   %sTK %.2f%s                                              %s\n", 
           COLOR_CYAN, COLOR_BRIGHT_CYAN, COLOR_RESET, COLOR_GREEN, total / expense_count, COLOR_RESET, COLOR_CYAN);
    printf("%s  %sTotal Entries:%s     %d%s                                                     %s\n", 
           COLOR_CYAN, COLOR_BRIGHT_CYAN, COLOR_RESET, expense_count, COLOR_RESET, COLOR_CYAN);
    printf("                                                                               \n");
    printf("%s===================================================================================%s\n", COLOR_BLUE, COLOR_RESET);
    printf("%s                      %s%sCATEGORY BREAKDOWN%s%s                               %s\n", 
           COLOR_CYAN, COLOR_BOLD, COLOR_BRIGHT_MAGENTA, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
    printf("%s===================================================================================%s\n", COLOR_BLUE, COLOR_RESET);
    
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
        printf("%s  %-20s: %sTK %-10.2f%s (%s%5.1f%%%s)                           %s\n", 
               COLOR_CYAN, categories[i], COLOR_BRIGHT_GREEN, category_totals[i], COLOR_RESET,
               COLOR_BRIGHT_YELLOW, percentage, COLOR_RESET, COLOR_CYAN);
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
    
    printf("                                                                               \n");
    printf("%s===================================================================================%s\n", COLOR_BLUE, COLOR_RESET);
    printf("%s  %sHighest Expense:%s %sTK %.2f%s                                              %s\n", 
           COLOR_CYAN, COLOR_BRIGHT_RED, COLOR_RESET, COLOR_BRIGHT_GREEN, highest.amount, COLOR_RESET, COLOR_CYAN);
    printf("%s     Category: %s, Description: %s                  %s\n", 
           COLOR_CYAN, highest.category, highest.description, COLOR_RESET);
    printf("                                                                               \n");
    printf("%s  %sLowest Expense:%s  %sTK %.2f%s                                              %s\n", 
           COLOR_CYAN, COLOR_BRIGHT_CYAN, COLOR_RESET, COLOR_GREEN, lowest.amount, COLOR_RESET, COLOR_CYAN);
    printf("%s     Category: %s, Description: %s                  %s\n", 
           COLOR_CYAN, lowest.category, lowest.description, COLOR_RESET);
    printf("%s===================================================================================%s\n", COLOR_BLUE, COLOR_RESET);
}

void save_to_file() {
    FILE *file = fopen(FILENAME, "wb");
    if (file == NULL) {
        printf("%sError: Could not save data to file!%s\n", COLOR_RED, COLOR_RESET);
        return;
    }
    
    // Save expense count first
    fwrite(&expense_count, sizeof(int), 1, file);
    
    // Save all expenses
    for (int i = 0; i < expense_count; i++) {
        fwrite(&expenses[i], sizeof(Expense), 1, file);
    }
    
    fclose(file);
    printf("%sData saved successfully to '%s'!%s\n", COLOR_BRIGHT_GREEN, FILENAME, COLOR_RESET);
}

void load_from_file() {
    FILE *file = fopen(FILENAME, "rb");
    if (file == NULL) {
        printf("%sNo previous data found. Starting fresh.%s\n", COLOR_YELLOW, COLOR_RESET);
        return;
    }
    
    // Read expense count
    fread(&expense_count, sizeof(int), 1, file);
    
    // Read all expenses
    for (int i = 0; i < expense_count; i++) {
        fread(&expenses[i], sizeof(Expense), 1, file);
    }
    
    fclose(file);
    printf("%sData loaded successfully from '%s'! (%d expenses)%s\n", COLOR_BRIGHT_GREEN, FILENAME, expense_count, COLOR_RESET);
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
    GetConsoleMode(hConsole, &mode);
    SetConsoleMode(hConsole, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    #endif
}
