# Expense Tracker

A simple yet powerful command-line expense tracking application written in C. Track, manage, and analyze your personal expenses with ease.

## Features

- ✨ **Add Expenses** - Record new expenses with date, amount, category, and description
- 📋 **View All Expenses** - Display all recorded expenses in a formatted table
- 🏷️ **Category Filtering** - View expenses filtered by specific categories
- 📅 **Date Range Queries** - View expenses within a specified date range
- 🔍 **Search Functionality** - Search expenses by description or category
- ✏️ **Modify Expenses** - Update existing expense details
- 🗑️ **Delete Expenses** - Remove unwanted expense records
- 📊 **Statistics Dashboard** - View comprehensive spending statistics including:
  - Total and average expenses
  - Category breakdown with percentages
  - Highest and lowest expenses
- 💾 **Persistent Storage** - All data is automatically saved to a file
- 🎨 **Colorful Interface** - Enhanced visual experience with color-coded text:
  - Green for monetary amounts
  - Red for errors and delete operations
  - Yellow for warnings and information
  - Cyan/Blue for headers and statistics
  - Color-coded menu options for easy navigation

## Requirements

- C compiler (GCC, Clang, or MSVC)
- Standard C libraries

## Installation

### Compile from Source

**Windows (MinGW/GCC):**
```bash
gcc expense.c -o expense.exe
```

**Linux/macOS:**
```bash
gcc expense.c -o expense
```

## Usage

Run the compiled executable:

**Windows:**
```bash
.\expense.exe
```

**Linux/macOS:**
```bash
./expense
```

### Menu Options

The application provides an interactive menu with the following options:

1. **Add New Expense** - Enter expense details (date, amount, category, description)
2. **View All Expenses** - Display all expenses with total
3. **View Expenses by Category** - Filter and view expenses for a specific category
4. **View Expenses by Date Range** - View expenses between two dates
5. **Search Expenses** - Search by keywords in description or category
6. **Modify Expense** - Update an existing expense by ID
7. **Delete Expense** - Remove an expense by ID
8. **Show Statistics** - View detailed spending analytics
9. **Exit** - Save and exit the application

### Example Usage

#### Adding an Expense
```
Enter date (YYYY-MM-DD) or 'today' for current date: today
Enter amount: 45.99
Enter category: Groceries
Enter description: Weekly shopping at supermarket
```

#### Viewing Statistics
```
=== Expense Statistics ===
Total Expenses: $523.45
Average Expense: $52.35

Category Breakdown:
-------------------
Groceries        : $234.50  ( 44.8%)
Transportation   : $120.00  ( 22.9%)
Entertainment    : $168.95  ( 32.3%)

Highest Expense: $89.99 (Entertainment - Concert tickets)
Lowest Expense: $12.50 (Transportation - Bus fare)
```

## Data Storage

- Expenses are automatically saved to `expenses.dat` in binary format
- Data persists between sessions
- File is created automatically on first run

## Technical Details

### Data Structure
```c
typedef struct {
    int id;
    char date[11];          // YYYY-MM-DD format
    float amount;
    char category[50];
    char description[100];
} Expense;
```

### Limits
- Maximum expenses: 1,000
- Category length: 50 characters
- Description length: 100 characters

## Development

### Project Structure
```
Expense-Tracker/
├── expense.c       # Main source code
├── expense.exe     # Compiled executable (Windows)
├── expenses.dat    # Data file (auto-generated)
├── .gitignore      # Git ignore rules
└── README.md       # This file
```

### Building for Different Platforms

**Windows (MSVC):**
```bash
cl expense.c /Fe:expense.exe
```

**With optimizations:**
```bash
gcc -O2 expense.c -o expense
```

## Contributing

Contributions are welcome! Feel free to:
- Report bugs
- Suggest new features
- Submit pull requests

## License

This project is open source and available for personal and educational use.

## Author

Masud Ibn Musa

## Roadmap

Future improvements planned:
- [ ] Export data to CSV/JSON
- [ ] Import expenses from files
- [ ] Budget tracking and alerts
- [ ] Monthly/yearly reports
- [ ] Multi-currency support
- [ ] Graphical charts (if GUI added)

## Contact

For questions or suggestions, please open an issue on GitHub.
