#include <iostream>
#include <string.h>
#include <fstream>
#include <ctime>
#include <vector>
#include <sstream> 
#include <windows.h>
#include <iomanip>
#include <algorithm>

using namespace std;

//constants
string MEAL_KITS_FILE = "mealKits.txt";
string ORDERS_FILE = "orders.txt";
int DELIVERY_CHARGE = 350;
enum Dif {HARD, MEDIUM, EASY};
//colorizing console outputs
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

Dif getDifFromText(string text){
  switch (stoi(text))
  {
  case 0: 
    return HARD;
  case 1:
    return MEDIUM;
  case 2:
    return EASY;
  default:
    return MEDIUM;
  }
}

void printDifs(){
  SetConsoleTextAttribute(hConsole, 11);
  cout << "Difficulty options: \n";
  SetConsoleTextAttribute(hConsole, DEFAULT_PALETTE);
  cout << "0 : HARD \n";
  cout << "1 : MEDIUM \n";
  cout << "2 : EASY \n \n";
}

string convertToString(char* a, int size) {
    int i;
    string s = "";
    for (i = 0; i < size; i++) {
        s = s + a[i];
    }
    return s;
}

//classes 
class Meal
{
private:
  string mealName;
  string description;
  string recipe;
  Dif difficulty;
  bool isVeg;
  int mealPrice;
  int timeTaken;

public:
    Meal(string mealName, Dif difficulty, int mealPrice, bool isVeg){
      this->mealName = mealName;
      this->difficulty = difficulty;
      this->mealPrice = mealPrice;
      this->isVeg = isVeg;
      this->description = "No description.";
      this->recipe = "No recipe.";
      this->timeTaken = 15;
    }

    string getMealName()
    {
      return this->mealName;
    }

    void setMealName(string mealName)
    {
      this->mealName = mealName;
    }

    string getDescription()
    {
      return this->description;
    }

    void setDescription(string description)
    {
      this->description = description;
    }

    string getRecipe()
    {
      return this->recipe;
    }

    void setRecipe(string recipe)
    {
      this->recipe = recipe;
    }

    Dif getDifficulty()
    {
      return this->difficulty;
    }

    void setDifficulty(Dif difficulty)
    {
      this->difficulty = difficulty;
    }

    bool isIsVeg()
    {
      return this->isVeg;
    }

    void setIsVeg(bool isVeg)
    {
      this->isVeg = isVeg;
    }

    int getMealPrice()
    {
      return this->mealPrice;
    }

    void setMealPrice(int mealPrice)
    {
      this->mealPrice = mealPrice;
    }

    int getTimeTaken()
    {
      return this->timeTaken;
    }

    void setTimeTaken(int timeTaken)
    {
      this->timeTaken = timeTaken;
    }

    ~Meal(){}
};

class Order
{
private:
  string mealName;
  string userName;
  string address;
  string orderNumber;
  string timestamp;
  int cost;

public:
  Order(string mealName, string userName, string address, int cost){
    this->mealName = mealName;
    this->userName = userName;
    this->address = address;
    //Standard delivery charge added to bill.
    this->cost = cost + DELIVERY_CHARGE;
    this->orderNumber = to_string(time(0));
    //Stamping current time.
    time_t now = time(0);
    char str[26];
    ctime_s(str, sizeof str, &now);
    string time = convertToString(str, sizeof str);
    time.pop_back();
    time.pop_back();
    this->timestamp = time;
  }

  string getMealName()
  {
    return this->mealName;
  }

  void setMealName(string mealName)
  {
    this->mealName = mealName;
  }

  string getUserName()
  {
    return this->userName;
  }

  void setUserName(string userName)
  {
    this->userName = userName;
  }

  string getAddress()
  {
    return this->address;
  }

  void setAddress(string address)
  {
    this->address = address;
  }

  string getOrderNumber()
  {
    return this->orderNumber;
  }

  void setOrderNumber(string orderNumber)
  {
    this->orderNumber = orderNumber;
  }

  string getTimestamp()
  {
    return this->timestamp;
  }

  void setTimestamp(string timestamp)
  {
    this->timestamp = timestamp;
  }

  int getCost()
  {
    return this->cost;
  }

  void setCost(int cost)
  {
    this->cost = cost;
  }

  ~Order(){}
};

//global variables
vector<Meal> mealKits;
vector<Order> allOrders;
bool isAdmin;

//Reading from text files
void readMealsList(){
  fstream mealKitsFile;
  mealKitsFile.open(MEAL_KITS_FILE, ios::in);
  if(mealKitsFile.is_open()){
    string line;
    while (getline(mealKitsFile,line))
    {
      stringstream ss(line);
      vector<string> v;

      while (ss.good()) {
        string substr;
        getline(ss, substr, '$');
        v.push_back(substr);
      }

      string mealName = v[0];
      string description = v[1];
      string recipe = v[2];
      Dif difficulty = getDifFromText(v[3]);
      bool isVeg = stoi(v[4]);
      int mealPrice = stoi(v[5]);
      int timeTaken = stoi(v[6]);

      Meal tempMeal(mealName, difficulty, mealPrice, isVeg);
      tempMeal.setDescription(description);
      tempMeal.setRecipe(recipe);
      tempMeal.setTimeTaken(timeTaken);

      mealKits.push_back(tempMeal);
    }
    
  }else{
    cout << '\n' << "Failed to read from meal kits file." << '\n';
  }
  mealKitsFile.close();
}

void readAllOrders(){
  fstream ordersFile;
  ordersFile.open(ORDERS_FILE, ios::in);
  if(ordersFile.is_open()){
    string line;
    while (getline(ordersFile,line))
    {
      stringstream ss(line);
      vector<string> v;

      while (ss.good()) {
        string substr;
        getline(ss, substr, '$');
        v.push_back(substr);
      }

      string mealName = v[0];
      string userName = v[1];
      string address = v[2];
      string orderNum = v[3];
      string timeStamp = v[4];
      int cost = stoi(v[5]);

      Order order(mealName, userName, address, 0);
      order.setCost(cost - DELIVERY_CHARGE);
      order.setOrderNumber(orderNum);
      order.setTimestamp(timeStamp);
      allOrders.push_back(order);
    }
  }else{
    cout << '\n' << "Failed to read from orders file." << '\n';
  }
  ordersFile.close();
}

//Writing to text files
bool saveMealsList(){
  fstream mealKitsFile;
  mealKitsFile.open(MEAL_KITS_FILE, ios::out);
  if(mealKitsFile.is_open()){
    for(Meal &meal : mealKits){
      mealKitsFile  << meal.getMealName() << '$' << meal.getDescription() << '$' << meal.getRecipe() << '$' << meal.getDifficulty() << '$' << meal.isIsVeg() << '$' << meal.getMealPrice() << '$' << meal.getTimeTaken() << "\n";
    }
  }else{
    cout << "\n" << "Failed to persist data to file." << "\n";
    return false;
  }
  mealKitsFile.close();
  return true;
}

bool saveOrders(){
  fstream ordersF;
  ordersF.open(ORDERS_FILE, ios::out);
  if(ordersF.is_open()){
    for(Order &order : allOrders){
      ordersF  << order.getMealName() << '$' << order.getUserName() << '$' << order.getAddress() << '$' << order.getOrderNumber() << '$' << order.getTimestamp() << '$' << order.getCost() << "\n";
    }
  }else{
    cout << "\n" << "Failed to persist data to file." << "\n";
    return false;
  }
  ordersF.close();
  return true;

}

//UI Components
class UI{
  public:
    static void commonDisplay(string fileName){
      fstream file;
      file.open(fileName, ios::in);
      SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
      cout << '\n';
      if(file.is_open()){
        string tempLine;
        while (getline(file, tempLine))
        {
          cout << tempLine << '\n';
        }
        file.close();
      }else{
        cout << "Failed to load UI Component" << '\n';
      }
      cout << '\n';
      SetConsoleTextAttribute(hConsole, DEFAULT_PALETTE);
    }

    static void displayWelcome(){
      commonDisplay("Components/Welcome.txt");
    }

    static void displaySeeU(){
      commonDisplay("Components/SeeU.txt");
    }
};

//Common methods
class Commons{
  public:
    static void log(string msg, int status){
      switch (status)
      {
      case 0:
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
        break;
      case 1:
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
        break;
      default:
        SetConsoleTextAttribute(hConsole, 11);
        break;
      }
      cout << msg << '\n';
      SetConsoleTextAttribute(hConsole, DEFAULT_PALETTE);
    }

    static void invalidInput(){
      SetConsoleTextAttribute(hConsole, FOREGROUND_RED); 
      cout << "Invalid input. Try again! \n";
      SetConsoleTextAttribute(hConsole, DEFAULT_PALETTE); 
    }

    static void displayMealKits(vector<Meal> &mealKitsArr){
      SetConsoleTextAttribute(hConsole, 11);
      cout << "   AVAILABLE MEAL KITS" << endl;
      SetConsoleTextAttribute(hConsole, 14);
      cout << setw(10) << left << "RefNo." << setw(25) << left << "Name" << setw(15) << left << "Veg/Non-veg" << setw(15) << left << "Price" << setw(15) << left << "Difficulty" << endl;
      SetConsoleTextAttribute(hConsole, DEFAULT_PALETTE);
      int i = 0;
      for (Meal &meal: mealKitsArr){
        cout << setw(10) << left << i << setw(25) << left << meal.getMealName();
        if(meal.isIsVeg()){
          SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
          cout << setw(15) << left << "VEG";
        }else{
          SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
          cout << setw(15) << left << "NON-VEG";
        }
        SetConsoleTextAttribute(hConsole, DEFAULT_PALETTE);
        cout << setw(15) << left << meal.getMealPrice();
        switch (meal.getDifficulty())
        {
          case HARD:
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
            cout << setw(15) << left << "HARD";
            break;
          case EASY:
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
            cout << setw(15) << left << "EASY";
            break;
          default:
            SetConsoleTextAttribute(hConsole, DEFAULT_PALETTE);
            cout << setw(15) << left << "MEDIUM";
            break;
        }
        SetConsoleTextAttribute(hConsole, DEFAULT_PALETTE); 
        i++;
        cout << '\n';
      }
      cout << '\n';
    }

    static void displayMeal(int index){
      //display mealKits[index]
      UI::commonDisplay("Components/MealIcon.txt");
      SetConsoleTextAttribute(hConsole, 14);
      cout << mealKits[index].getMealName() << '\n';
      SetConsoleTextAttribute(hConsole, DEFAULT_PALETTE); 
      cout << mealKits[index].getDescription() << '\n';

      cout << "Veg/Non-Veg : ";
      if(mealKits[index].isIsVeg()){
          SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
          cout << "VEG";
      }else{
          SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
          cout << "NON-VEG";
      }
      cout << '\n';

      SetConsoleTextAttribute(hConsole, DEFAULT_PALETTE); 
      cout << "Difficulty : ";
      switch (mealKits[index].getDifficulty())
        {
        case HARD:
          SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
          cout << "HARD";
          break;
        case EASY:
          SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
          cout << "EASY";
          break;
        default:
          SetConsoleTextAttribute(hConsole, DEFAULT_PALETTE);
          cout << "MEDIUM";
          break;
        }
      cout << '\n';

      SetConsoleTextAttribute(hConsole, DEFAULT_PALETTE); 
      cout << "Meal preparation time : " << mealKits[index].getTimeTaken() << "min" << '\n';
      cout << "Price : LKR " << mealKits[index].getMealPrice() << '\n';

      //open recipe
      if(yesNoInput("Open recipe?")){
        if(mealKits[index].getRecipe() != "No recipe."){
          string path = "Recipe/" + mealKits[index].getRecipe(); 
          UI::commonDisplay(path);
        }else{
          Commons::log(mealKits[index].getRecipe(), 0);
        }
      }
    }

    static bool yesNoInput(string qu){
      try
      {
        SetConsoleTextAttribute(hConsole, 14);
        cout << '\n' << qu << '\n';
        SetConsoleTextAttribute(hConsole, 8);
        cout << "(Type y for 'YES' or n for 'NO') : ";
        SetConsoleTextAttribute(hConsole, DEFAULT_PALETTE);
        string str;
        getline(cin, str);
        cout << '\n';
        if(str == "y"){
          return true;
        }else if( str == "n"){
          return false;
        }else{
          Commons::invalidInput();
          yesNoInput(qu);
          return false;
        }
      }
      catch(exception e)
      {
        exit(0);
      }
    }

    static int optionSelector(string qu){
      try
      { 
        SetConsoleTextAttribute(hConsole, 14);
        int option;
        cout << qu;
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
        cin >> option;
        SetConsoleTextAttribute(hConsole, DEFAULT_PALETTE);
        cin.ignore(10, '\n');
        cout << '\n';
        return option;
      }
      catch(exception e)
      {
        Commons::invalidInput();
        optionSelector(qu);
        return -1;
      }
    }

    static string getAnswer(string qu){
      SetConsoleTextAttribute(hConsole, 14);
      cout << qu << '\n';
      string tempString;
      SetConsoleTextAttribute(hConsole, DEFAULT_PALETTE);
      getline(cin, tempString);
      cout << '\n';
      return tempString;
    }

    static void displayOrders(vector<Order> &allOrders){
      SetConsoleTextAttribute(hConsole, 11);
      cout << "   ALL ORDERS" << endl;
      SetConsoleTextAttribute(hConsole, 14);
      cout << setw(15) << left << "OrderNo." << setw(15) << left << "Customer Name" << setw(25) << left << "Meal Kit" << setw(40) << left << "Delivery Address" << setw(15) << left << "Total" << setw(15) << left << "Timestamp" << endl;
      SetConsoleTextAttribute(hConsole, DEFAULT_PALETTE);
      for (Order &order: allOrders){
        cout << setw(15) << left << order.getOrderNumber();
        cout << setw(15) << left << order.getUserName();
        cout << setw(25) << left << order.getMealName();
        cout << setw(40) << left << order.getAddress();
        cout << setw(15) << left << order.getCost();
        cout << setw(15) << left << order.getTimestamp() << '\n';
        SetConsoleTextAttribute(hConsole, DEFAULT_PALETTE);
      }
      cout << '\n';      
    }

    static void displayOrder(Order &order){
      UI::commonDisplay("Components/Bill.txt");
      cout << "Order number : " << order.getOrderNumber() << '\n';
      SetConsoleTextAttribute(hConsole, 13);
      cout << "@ " << order.getTimestamp() << '\n';
      SetConsoleTextAttribute(hConsole, 14);
      cout << "Name : " << order.getUserName() << '\n';
      cout << "Meal kit : " << order.getMealName() << '\n';
      cout << "Delivery Address : " << order.getAddress() << '\n';
      SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
      cout << "Cost : " << order.getCost() << '\n';
      SetConsoleTextAttribute(hConsole, DEFAULT_PALETTE);
      cout << '\n';
    }
};

//Authentication
class Auth{
  public:
    static string encrypt(string pw){
      char str[8];
      strcpy_s(str, pw.c_str());
      for(int i = 0; (i < 8 && str[i] != '\0'); i++){
        str[i] = str[i] + 2;
      }
      return convertToString(str,sizeof(str));
    }

    static string decrypt(string pw){
      char str[8];
      strcpy_s(str, pw.c_str());
      for(int i = 0; (i < 8 && str[i] != '\0'); i++){
        str[i] = str[i] - 2;
      }
      return convertToString(str,sizeof(str));
    }

    static bool authenticate(string userInput){
      fstream pwFile;
      pwFile.open("Authenticate.txt", ios::in);
      if(pwFile.is_open()){
        string line;
        getline(pwFile,line);
        pwFile.close();
        return userInput == line;
      }
      pwFile.close();
      return false;
    }

    static bool changePassword(){
      string currentPw = Commons::getAnswer("Enter current password : ");
      if(authenticate(currentPw)){
        Commons::log("Password should contain 8 characters.", 1);
        string newPassword = Commons::getAnswer("Enter new passwrod : ");
        if(sizeof(newPassword) - 16 != 8){
          Commons::log("'Password should contain 8 characters' statement failed", 0);
          return false;
        }
        fstream pwFile;
        pwFile.open("Authenticate.txt", ios::out);
        pwFile << newPassword;
        pwFile.close();
        return true;
      }
      return false;
    }
};

//methods
bool addMeal(){
  string mealName = Commons::getAnswer("Enter meal name : ");   
  string description = Commons::getAnswer("Enter description : ");   
  int mealPrice = Commons::optionSelector("Enter meal price : ");
  bool isVeg = Commons::yesNoInput("Is meal veg?");
  printDifs();
  string dift = to_string(Commons::optionSelector("Enter meal difficulty : "));
  Dif dif = getDifFromText(dift);
  int time = Commons::optionSelector("Enter preparation time : ");
  string rec = Commons::getAnswer("Enter recipe file \n (ex => fileName.txt) : ");
  if(mealName == "" || mealPrice<=0 || time<=0){
    Commons::invalidInput();
    return false;
  }else{
    Meal meal(mealName, dif, mealPrice, isVeg);
    meal.setDescription(description);
    meal.setTimeTaken(time);
    meal.setRecipe(rec);
    mealKits.push_back(meal);
    return true;
  }   
}

void editMeal(int index){
  Commons::displayMeal(index);
  bool a = addMeal();
  if(a){
    mealKits.erase(mealKits.begin() + index);
  }
  
}

bool makeOrder(int index){
  bool yes = Commons::yesNoInput("Confirm order?");
  if(yes){
    string userName = Commons::getAnswer("Enter your name : ");
    string address = Commons::getAnswer("Enter delivery address : ");
    Commons::log(" *** Order confirmed ***", 1);
    string msg = "Delivery charge of " + to_string(DELIVERY_CHARGE) + " is added.";
    Commons::log(msg, 2);
    Order newOrder(mealKits[index].getMealName(), userName, address, mealKits[index].getMealPrice());
    Commons::displayOrder(newOrder);
    allOrders.push_back(newOrder);
    return true;
  }else{
    Commons::log("Order dismissed.",0);
    return false;
  }
}

void adminMenu(){
  Commons::log("\n Select an option to proceed : ", 1);
  cout << " 0 : Exit \n";
  cout << " 1 : Show all the meal kits \n";
  cout << " 2 : Show all the orders \n";
  cout << " 3 : Modify meal kits \n";
  cout << " 4 : Change password \n";
  cout << " 5 : Back \n \n";
}

void modifyMealKitsMenu(){
  Commons::log("\n Select an option to proceed : ", 1);
  cout << " 0 : Exit \n";
  cout << " 1 : Add a new meal kit \n";
  cout << " 2 : Edit an existing meal kit \n";
  cout << " 3 : Delete a meal kit \n";
  cout << " 4 : Back \n \n";
}

bool modifyMealKits(){
  while (true){
    modifyMealKitsMenu();
    int op = Commons::optionSelector("Select option :");
    if(op == 0){
      //exit
      return false;
    }else if(op == 1){
      //add mealkit
      addMeal();
      continue;
    }else if(op == 2){
      //edit mealkit
      int index = Commons::optionSelector("Enter meal RefNo. : ");
      if(index < 0 || index >= mealKits.size()){
        Commons::invalidInput();
      }else{
        editMeal(index);
      }
      continue;
    }else if(op == 3){
      //delete meal
      int index = Commons::optionSelector("Enter meal RefNo. : ");
      if(index < 0 || index >= mealKits.size()){
        Commons::invalidInput();
      }else{
        mealKits.erase(mealKits.begin() + index);
      }
      continue;
    }else if(op == 4){
      //back
      return true;
    }else{
      Commons::invalidInput();
      continue;
    }
  }
}

int isSubstring(string s1, string s2)
{
    int M = s1.length();
    int N = s2.length();

    for (int i = 0; i <= N - M; i++) {
        int j;

        for (j = 0; j < M; j++)
            if (s2[i + j] != s1[j])
                break;

        if (j == M)
            return i;
    }

    return -1;
}

vector<int> filteredMealKits(string &searchText) {
    int refNo = 0;
    vector<int> filteredResults;
    for (Meal& meal : mealKits) {
        string tempName = meal.getMealName();
        transform(tempName.begin(), tempName.end(), tempName.begin(), ::tolower);
        if (isSubstring(searchText, tempName)!=-1) {
            filteredResults.push_back(refNo);
        }
        refNo++;
    }
    //sort(filteredResults.begin(), filteredResults.end());
    return filteredResults;
}

void displaySearchResults(string &searchText) {
    SetConsoleTextAttribute(hConsole, 11);
    cout << "   SEARCH RESULTS" << endl;
    SetConsoleTextAttribute(hConsole, 14);
    cout << setw(10) << left << "RefNo." << setw(25) << left << "Name" << setw(15) << left << "Veg/Non-veg" << setw(15) << left << "Price" << setw(15) << left << "Difficulty" << endl;
    SetConsoleTextAttribute(hConsole, DEFAULT_PALETTE);
    for (int& i : filteredMealKits(searchText)) {
        cout << setw(10) << left << i << setw(25) << left << mealKits[i].getMealName();
        if (mealKits[i].isIsVeg()) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
            cout << setw(15) << left << "VEG";
        }
        else {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
            cout << setw(15) << left << "NON-VEG";
        }
        SetConsoleTextAttribute(hConsole, DEFAULT_PALETTE);
        cout << setw(15) << left << mealKits[i].getMealPrice();
        switch (mealKits[i].getDifficulty())
        {
        case HARD:
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
            cout << setw(15) << left << "HARD";
            break;
        case EASY:
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
            cout << setw(15) << left << "EASY";
            break;
        default:
            SetConsoleTextAttribute(hConsole, DEFAULT_PALETTE);
            cout << setw(15) << left << "MEDIUM";
            break;
        }
        SetConsoleTextAttribute(hConsole, DEFAULT_PALETTE);
        i++;
        cout << '\n';
    }
    cout << '\n';
}

bool adminUserFlow(){
    if(!isAdmin){
      isAdmin = Auth::authenticate(Commons::getAnswer("Enter master password"));
    }
    if(isAdmin){
      while (true)
      { 
        //MENU: back, mealkits, orders, exit, editmeals
        Commons::log("  ADMIN MENU", 2);
        adminMenu();
        int option = Commons::optionSelector("Select option : ");
        if(option == 0){
          //exit
          return false;             
        }else if (option == 1){
          //show mealKits method
          Commons::displayMealKits(mealKits);
          continue;
        }else if(option == 2){
          //show orders method
          Commons::log("  ALLORDERS", 2);
          Commons::displayOrders(allOrders);
          continue;
        }else if(option == 3){
          //modify meals method
          Commons::log("  MEAL KITS MENU", 2);
          if(modifyMealKits()){
            continue;
          }
          return false;
        }else if(option == 4){
          //change password
          Commons::log("  CHANGE MASTER PASSWORD \n", 2);
          Auth::changePassword();
          continue;
        }else if(option == 5){
           //back
          return true;
        }else{
          Commons::invalidInput();
          continue;
        }
      }
    }else{
      Commons::log("Authentication failed.", 0);
      return false;
    }
}

bool guestUserFlow(){
    Commons::displayMealKits(mealKits);
    while(true){
      if(Commons::yesNoInput("Custom search? \n => Press 'NO' to continue to order.")){
        cout << "Search" << endl;
        //search method
        string keyword = Commons::getAnswer("Enter search term: ");
        transform(keyword.begin(), keyword.end(), keyword.begin(), ::tolower);
        displaySearchResults(keyword);
        continue;
      }else{
        while (true){
          if(Commons::yesNoInput("Order now?")){
            int selection = Commons::optionSelector("Choose meal to order using RefNo. : ");
            if(selection < 0 || selection >= mealKits.size()){
              Commons::invalidInput();
              continue;
            }else{
              Commons::displayMeal(selection);
              makeOrder(selection);
              if(Commons::yesNoInput("Order again?")){
                continue;
              }else{
                break;
              }
            }
          }else{
            break;
          }
        }
        if(Commons::yesNoInput("Return to landing page \n=> ( Press 'NO' to exit.)?")){
          return true;
        }else{
          return false;
        }
      }
    }
    return true;
}


void runMain(){
    UI::displayWelcome();
    readMealsList();
    readAllOrders();

    while (true)
    {
      if(Commons::yesNoInput("***SIGN IN*** \n=> Select 'NO' to skip login. :")){
        bool a = adminUserFlow();
        if(a){
          continue;
        }else{
          break;
        }
      }else{
        if(guestUserFlow()){
          continue;
        }else{
          break;
        }
      }
    }
    saveOrders();
    saveMealsList();
    UI::displaySeeU();
    system("pause");
}

int main()  
{
    // cout << "Hello World" << endl;
    runMain();
}