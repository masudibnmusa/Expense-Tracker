#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_EXPENSES 1000
#define MAX_CATEGORY_LENGTH 50
#define MAX_DESCRIPTION_LENGTH 100
#define FILENAME "expenses.dat"

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
    
    printf("=== Welcome to Expense Tracker ===\n");
    load_from_file();
    
    while (1) {
        display_menu();
        printf("Enter your choice (1-9): ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            clear_input_buffer();
            continue;
        }
        
        switch (choice) {
            case 1:
                add_expense();
                break;
            case 2:
                view_all_expenses();
                break;
            case 3:
                view_by_category();
                break;
            case 4:
                view_by_date_range();
                break;
            case 5:
                search_expenses();
                break;
            case 6:
                modify_expense();
                break;
            case 7:
                delete_expense();
                break;
            case 8:
                show_statistics();
                break;
            case 9:
                save_to_file();
                printf("Thank you for using Expense Tracker. Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice! Please enter a number between 1-9.\n");
        }
    }
    
    return 0;
}

void display_menu() {
    printf("\n=== Expense Tracker Menu ===\n");
    printf("1. Add New Expense\n");
    printf("2. View All Expenses\n");
    printf("3. View Expenses by Category\n");
    printf("4. View Expenses by Date Range\n");
    printf("5. Search Expenses\n");
    printf("6. Modify Expense\n");
    printf("7. Delete Expense\n");
    printf("8. Show Statistics\n");
    printf("9. Exit\n");
}

void add_expense() {
    if (expense_count >= MAX_EXPENSES) {
        printf("Error: Expense tracker is full! Cannot add more expenses.\n");
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
            printf("Invalid date format! Please use YYYY-MM-DD format.\n");
        }
    }
    
    // Get amount
    while (1) {
        printf("Enter amount: ");
        if (scanf("%f", &new_expense.amount) == 1 && new_expense.amount > 0) {
            break;
        } else {
            printf("Invalid amount! Please enter a positive number.\n");
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
    printf("\nExpense added successfully! (ID: %d)\n", new_expense.id);
}

void view_all_expenses() {
    if (expense_count == 0) {
        printf("No expenses recorded.\n");
        return;
    }
    
    printf("\n%-5s %-12s %-12s %-20s %-30s\n", 
           "ID", "Date", "Amount", "Category", "Description");
    printf("===============================================================================\n");
    
    float total = 0;
    for (int i = 0; i < expense_count; i++) {
        printf("%-5d %-12s $%-11.2f %-20s %-30s\n",
               expenses[i].id,
               expenses[i].date,
               expenses[i].amount,
               expenses[i].category,
               expenses[i].description);
        total += expenses[i].amount;
    }
    printf("===============================================================================\n");
    printf("%-51s $%-11.2f\n", "TOTAL EXPENSES:", total);
}

void view_by_category() {
    if (expense_count == 0) {
        printf("No expenses recorded.\n");
        return;
    }
    
    char category[MAX_CATEGORY_LENGTH];
    printf("Enter category to view: ");
    clear_input_buffer();
    fgets(category, MAX_CATEGORY_LENGTH, stdin);
    category[strcspn(category, "\n")] = 0;
    
    printf("\nExpenses in category '%s':\n", category);
    printf("%-5s %-12s %-12s %-30s\n", "ID", "Date", "Amount", "Description");
    printf("==========================================================\n");
    
    float category_total = 0;
    int found = 0;
    
    for (int i = 0; i < expense_count; i++) {
        if (strcasecmp(expenses[i].category, category) == 0) {
            printf("%-5d %-12s $%-11.2f %-30s\n",
                   expenses[i].id,
                   expenses[i].date,
                   expenses[i].amount,
                   expenses[i].description);
            category_total += expenses[i].amount;
            found = 1;
        }
    }
    
    if (found) {
        printf("==========================================================\n");
        printf("%-29s $%-11.2f\n", "CATEGORY TOTAL:", category_total);
    } else {
        printf("No expenses found in category '%s'\n", category);
    }
}

void view_by_date_range() {
    if (expense_count == 0) {
        printf("No expenses recorded.\n");
        return;
    }
    
    char start_date[11], end_date[11];
    
    printf("Enter start date (YYYY-MM-DD): ");
    scanf("%s", start_date);
    if (!validate_date(start_date)) {
        printf("Invalid start date format!\n");
        return;
    }
    
    printf("Enter end date (YYYY-MM-DD): ");
    scanf("%s", end_date);
    if (!validate_date(end_date)) {
        printf("Invalid end date format!\n");
        return;
    }
    
    printf("\nExpenses from %s to %s:\n", start_date, end_date);
    printf("%-5s %-12s %-12s %-20s %-30s\n", "ID", "Date", "Amount", "Category", "Description");
    printf("===============================================================================\n");
    
    float period_total = 0;
    int found = 0;
    
    for (int i = 0; i < expense_count; i++) {
        if (strcmp(expenses[i].date, start_date) >= 0 && 
            strcmp(expenses[i].date, end_date) <= 0) {
            printf("%-5d %-12s $%-11.2f %-20s %-30s\n",
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
        printf("===============================================================================\n");
        printf("%-51s $%-11.2f\n", "PERIOD TOTAL:", period_total);
    } else {
        printf("No expenses found in the specified date range.\n");
    }
}

void search_expenses() {
    if (expense_count == 0) {
        printf("No expenses recorded.\n");
        return;
    }
    
    char search_term[MAX_DESCRIPTION_LENGTH];
    printf("Enter search term (description or category): ");
    clear_input_buffer();
    fgets(search_term, MAX_DESCRIPTION_LENGTH, stdin);
    search_term[strcspn(search_term, "\n")] = 0;
    
    printf("\nSearch results for '%s':\n", search_term);
    printf("%-5s %-12s %-12s %-20s %-30s\n", "ID", "Date", "Amount", "Category", "Description");
    printf("===============================================================================\n");
    
    float search_total = 0;
    int found = 0;
    
    for (int i = 0; i < expense_count; i++) {
        if (strstr(expenses[i].description, search_term) != NULL ||
            strstr(expenses[i].category, search_term) != NULL) {
            printf("%-5d %-12s $%-11.2f %-20s %-30s\n",
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
        printf("===============================================================================\n");
        printf("%-51s $%-11.2f\n", "SEARCH RESULTS TOTAL:", search_total);
    } else {
        printf("No expenses found matching your search.\n");
    }
}

void modify_expense() {
    if (expense_count == 0) {
        printf("No expenses recorded.\n");
        return;
    }
    
    int id;
    printf("Enter expense ID to modify: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID!\n");
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
        printf("Expense with ID %d not found.\n", id);
        return;
    }
    
    printf("\nCurrent expense details:\n");
    printf("Date: %s\n", expenses[found].date);
    printf("Amount: $%.2f\n", expenses[found].amount);
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
    
    printf("Expense modified successfully!\n");
}

void delete_expense() {
    if (expense_count == 0) {
        printf("No expenses recorded.\n");
        return;
    }
    
    int id;
    printf("Enter expense ID to delete: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID!\n");
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
        printf("Expense with ID %d not found.\n", id);
        return;
    }
    
    printf("\nExpense to delete:\n");
    printf("ID: %d\n", expenses[found].id);
    printf("Date: %s\n", expenses[found].date);
    printf("Amount: $%.2f\n", expenses[found].amount);
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
        printf("Expense deleted successfully!\n");
    } else {
        printf("Deletion cancelled.\n");
    }
}

void show_statistics() {
    if (expense_count == 0) {
        printf("No expenses recorded.\n");
        return;
    }
    
    printf("\n=== Expense Statistics ===\n");
    
    // Total expenses
    float total = 0;
    for (int i = 0; i < expense_count; i++) {
        total += expenses[i].amount;
    }
    printf("Total Expenses: $%.2f\n", total);
    
    // Average expense
    printf("Average Expense: $%.2f\n", total / expense_count);
    
    // Category breakdown
    printf("\nCategory Breakdown:\n");
    printf("-------------------\n");
    
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
        printf("%-20s: $%-8.2f (%5.1f%%)\n", 
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
    
    printf("\nHighest Expense: $%.2f (%s - %s)\n", 
           highest.amount, highest.category, highest.description);
    printf("Lowest Expense: $%.2f (%s - %s)\n", 
           lowest.amount, lowest.category, lowest.description);
}

void save_to_file() {
    FILE *file = fopen(FILENAME, "wb");
    if (file == NULL) {
        printf("Error: Could not save data to file!\n");
        return;
    }
    
    // Save expense count first
    fwrite(&expense_count, sizeof(int), 1, file);
    
    // Save all expenses
    for (int i = 0; i < expense_count; i++) {
        fwrite(&expenses[i], sizeof(Expense), 1, file);
    }
    
    fclose(file);
    printf("Data saved successfully to '%s'!\n", FILENAME);
}

void load_from_file() {
    FILE *file = fopen(FILENAME, "rb");
    if (file == NULL) {
        printf("No previous data found. Starting fresh.\n");
        return;
    }
    
    // Read expense count
    fread(&expense_count, sizeof(int), 1, file);
    
    // Read all expenses
    for (int i = 0; i < expense_count; i++) {
        fread(&expenses[i], sizeof(Expense), 1, file);
    }
    
    fclose(file);
    printf("Data loaded successfully from '%s'! (%d expenses)\n", FILENAME, expense_count);
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