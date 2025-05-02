# include <iostream>
# include <cstdlib>
# include <vector>
# include <string>
# include <fstream>
# include <iomanip>

using namespace std;

enum enColumnType { Qualitative = 1, Quantitative = 2};

enum enFeature { EditColumn = 1, GenerateData = 2 , ShowAllColumns = 3};

enum enEditColumnsFeatures { AddColumnOption = 1, EditColumnOption = 2, DeleteColumnOption = 3 };

void ShowMainMenu();


long long int ReadNumber()
{
	long long Number;

	cin >> Number;
	while (cin.fail())
	{
		// user didn't input a number
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		cout << "Invalid Number, Enter a valid one:" << endl;
		cin >> Number;
	}
	return Number;
}

struct stColumns
{
	string Name;
	enColumnType DataType;
	bool MarkForDelete = false;
	vector <string> PossibleValues; // incase of nominal oand ordinal values 
	
	// incase of numberical values
	long long StartRange = 0;
	long long EndRange = 0;
};

void PressAnyKey(string Destination)
{
	cout << "\n press any key to go to " << Destination;
	system("pause>0");
}

vector <stColumns> AllColumns;

string FileName = "GeneratedData.txt";
const string Red = "\033[31m";
const string Green = "\033[32m";
const string ResetDefault = "\033[0m";

bool IsColumnFound(string ColumnName)
{
	for (stColumns& col : AllColumns)
	{
		if (col.Name == ColumnName)
		{
			return true;
		}
	}
	return false;
}


bool MarkForDeletion(string ColumnName)
{
	for (stColumns& col : AllColumns)
	{
		if (col.Name == ColumnName)
		{
			col.MarkForDelete = true;
			return true;
		}
	}
	return false;
}

long long RandomNumber(long long From, long long To)
{
	long long Number;
	Number = rand() % (To - From + 1) + From;
	return Number;
}


string ReadString(string Message)
{
	cout << Message ;
	string Word;
	getline(cin >> ws, Word);
	return Word;
}


bool AskForConfirm(string Message)
{
	string AddMore;
	cout << Message;
	cin >> AddMore;
	return (toupper(AddMore[0]) == 'Y');
}



string PrintColumnType(stColumns Column)
{
	switch (Column.DataType)
	{
	case enColumnType::Qualitative:
	{
		return "(Qualitative)";
		break;
	}
	case enColumnType::Quantitative:
	{
		return "(Quantitative)";
		break;
	}
	
	}
		return"";
}

enColumnType ReadColumnType()
{
	short Type;
	bool Valid = true;
	cout << "\nPlease enter the column type : \n";
	do
	{
		if (!Valid)
		{
			cout << Red << "\nInvalid Option , Please choose valid option only [ 1 , 2 ]" << ResetDefault << endl;
		}
		cout << "[1] Qualitive (strings)  " << endl;
		cout << "[2] Quantitve (Numbers) " << endl;
		
		 Type = ReadNumber();
		Valid = false;
	} while (Type != 1 && Type != 2 );

	return (enColumnType)Type;
}

stColumns ReadColumnData()
{
	stColumns Column;
	bool FirstTry = true;
	do
	{
		if (!FirstTry)
		{
			cout << "\n This column is already exsists ! " << endl;
		}
		Column.Name = ReadString("\nPlease enter column name : ");
	} while (IsColumnFound(Column.Name));
	Column.DataType = ReadColumnType();
	return Column;
}

string ConverColumnToLine(stColumns Column)
{
	string Line = "";
	Line += Column.Name + " ";
	switch (Column.DataType)
	{
		case enColumnType::Qualitative:
		{
			Line += "(string)";
			break;
		}
		case enColumnType::Quantitative:
		{
			Line += "(Numberical)";
			break;
		}
		
		default:
		{
			cout << "Invalid\n";
			break;
		}
	}

	return Line;
}

enFeature ReadFeatureOption()
{
	short Choice;
	bool Valid = true;

	do
	{
		if (!Valid)
		{
			cout << Red<< "\nInvalid Option , Please choose valid option only [ 1 , 2 , 3 ]"<<ResetDefault << endl;
		}
		cout << "\n[1] Edit columns ( Add , Update , Delete ) " << endl;
		cout << "[2] Generate Data " << endl;
		cout << "[3] List all columns " << endl;
		 Choice = ReadNumber();
		Valid = false;
	} while (Choice != 1 && Choice != 2 && Choice != 3 );
	
	return (enFeature)Choice;
}


enEditColumnsFeatures ReadEditColumnOption()
{
	short Type;
	bool Valid = true;
	cout << "\nPlease enter the option you want : \n";
	do
	{
		if (!Valid)
		{
			cout << Red << "\nInvalid Option , Please choose valid option only [ 1 , 2 , 3 ]" << ResetDefault << endl;
		}
		cout << "[1] Add Column  " << endl;
		cout << "[2] Edit Column  " << endl;
		cout << "[3] Delete Column " << endl;
		 Type = ReadNumber();
		Valid = false;
	} while (Type != 1 && Type != 2 && Type != 3);

	return (enEditColumnsFeatures)Type;
}

void ListAllColumns()
{
	system("cls");
	int Number = 1;
	if (AllColumns.size() == 0)
	{
		cout << "\n You have NO Columns \n " << endl;
		PressAnyKey("Main menu...");
		ShowMainMenu();
		return;
	}
	cout << Green << "=============================================\n";
	cout << ResetDefault << "List all Column Screen  , you have "<< AllColumns.size() <<" columns" << "\n";
	cout << Green << "=============================================\n" << ResetDefault;
	for (stColumns& col : AllColumns)
	{
		cout << "[ " << Number << " ] " << col.Name << " " << PrintColumnType(col) << endl;
		Number++;
	}
}


void AddColumnOptionScreen() {
	do
	{
		system("cls");
		cout << Green << "====================================\n";
		cout << ResetDefault << "Add Column Screen \n";
		cout << Green << "====================================\n" << ResetDefault;
		stColumns Column;
		Column = ReadColumnData();
		AllColumns.push_back(Column);
		
	} while (AskForConfirm("\nDo You want to add more ? [Y/N] : "));

	if (AskForConfirm("\n Do you want to list all columns after the adding ? [Y / N ] : "))
	{
		system("cls");
		ListAllColumns();
	}
}




stColumns EditColumnInformation(stColumns Column)
{
	string Name = ReadString("Enter column name : ");
	if (!IsColumnFound(Name) || Name == Column.Name)
	{
		Column.Name = Name;
		Column.DataType = ReadColumnType();
		return Column;
	}
	else
	{
		cout << "\n Columns names must be unique";
	}
	return Column;
}

bool EditSelectedColumn(string ColumnName)
{
	for (stColumns& col : AllColumns)
	{
		if (col.Name == ColumnName)
		{
			col = EditColumnInformation(col);
			return true;
		}
	}
	return false;
}


void EditColumnOptionScreen()
{
	system("cls");
	ListAllColumns();

	cout << Green << "====================================\n";
	cout << ResetDefault << "Edit Column Screen \n";
	cout << Green << "====================================\n" << ResetDefault;

	string ColumnName = ReadString(" \nEnter column name you want to edit : ");


	if (AskForConfirm("\n Are you sure you want to edit this column ? [ Y / N ] : "))
	{
		if (EditSelectedColumn(ColumnName))
		{
			cout << "\n Column updated successfully ! " << endl;
		}
		else
		{
			cout << "Column is NOT found ! " << endl;
		}
	}
	else
	{
		cout << "\nEdit canceled ";
	}
	
}

vector<stColumns> UpdateAfterDeleteColumnOptionScreen()
{
	vector<stColumns> NewColumns;

	for (int i = 0; i < AllColumns.size(); i++)
	{
		if (AllColumns[i].MarkForDelete == false)
		{
			NewColumns.push_back(AllColumns[i]);
		}
	}
	AllColumns = NewColumns;
	return AllColumns;
}

void DeleteColumnOptionScreen()
{
	system("cls");
	ListAllColumns();

	cout << Green << "====================================\n";
	cout << ResetDefault << "Delete Column Screen \n";
	cout << Green << "====================================\n" << ResetDefault;

	string ColumnName = ReadString(" \nEnter column name you want to delete : ");

	if (AskForConfirm("\n Are you sure you want to delete this column ? [ Y / N ] : "))
	{
		if (MarkForDeletion(ColumnName))
		{

			AllColumns = UpdateAfterDeleteColumnOptionScreen();
			cout << " Column deleted successfully ! " << endl;


		}
		else
		{
			cout << " \n Column does NOT exsists ! " << endl;
		}
	}
	else
	{
		cout << "\nDeletion canceled ";
	}

}



void PerformEditColumnsOption(enEditColumnsFeatures Option)
{
	switch (Option)
	{
	case enEditColumnsFeatures::AddColumnOption :
	{
		AddColumnOptionScreen();
		break;
	}

	case enEditColumnsFeatures::EditColumnOption :
	{
		EditColumnOptionScreen();
		break;
	}
	
	case enEditColumnsFeatures::DeleteColumnOption :
	{
		DeleteColumnOptionScreen();
		break;
	}

	}
}

void ShowEditCoulmnsScreen()
{
	system("cls");
	enEditColumnsFeatures Option;
	Option = ReadEditColumnOption();

	PerformEditColumnsOption(Option);
}

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

vector <string> SetAllPossibleValues()
{
	vector <string> PossibleValues;
	do
	{
		string Value = ReadString("Please enter the possible value : ");
		PossibleValues.push_back(Value);
	} while (AskForConfirm("\nDo you want to add more  [ Y / N ] ? : "));
	return PossibleValues;
}

void PrepareValues()
{
	
	string Answer;
	for (stColumns& Column : AllColumns)
	{
		system("cls");
		cout << Green << "============================================================\n";
		cout << ResetDefault << " Generate Prepared Values ";
		cout << Green << "\n============================================================\n" << ResetDefault;
		cout << "What is the nature of values of this Column : " <<Red<< Column.Name <<ResetDefault<< " [(1) string, (2) number] ? \n ";
		cout << "Answer with numbers or the word (case sensitive) : \n ";
		cin >> Answer;
		if (Answer == "1" || Answer == "string")
		{
			cout << "\nEntering All Possible values for " << Column.Name << endl << endl;
			Column.PossibleValues = SetAllPossibleValues();

		}

		else if (Answer == "2" || Answer == "number")
		{
			do
			{
				cout << "\nEntering  Possible values range for " << Column.Name << endl << endl;
				cout << "Enter minimum value : ";
				Column.StartRange = ReadNumber();
				cout << "Enter maximum value : ";
				Column.EndRange = ReadNumber();
			} while (Column.StartRange > Column.EndRange);
			


		}
		else
		{
			cout << "\n" << Red << "Invalid inputs ! " << ResetDefault << endl;
			PressAnyKey(" termination ...");

		}

	}
}

string GenerateAValue(stColumns Column)
{
	if (Column.PossibleValues.size() == 0)
	{
		string Number;
		Number = to_string(RandomNumber(Column.StartRange, Column.EndRange));
		
		return Number;
	}
	else
	{
		return Column.PossibleValues[RandomNumber(0, Column.PossibleValues.size() - 1)];
	}
}

string GenerateRow(vector<stColumns> AllColumns)
{
	string Line = "";
	for (stColumns& Col : AllColumns)
	{
		Line += GenerateAValue(Col)+"|";
	}
	Line.pop_back();
	Line.push_back(';');
	return Line;
}


void GenerateEntireData(long long NumberOfRows)
{
	string Line;
	fstream MyFile;
	MyFile.open(FileName, ios::out);
	if (MyFile.is_open())
	{
		// Columns Names
		for (stColumns& col : AllColumns)
		{
			Line += col.Name + "|";
		}
		Line.pop_back();
		Line.push_back(';');
		MyFile << Line << endl;

		Line = "";
		for (int i = 1; i <= NumberOfRows; i++)
		{
			Line = GenerateRow(AllColumns);
			MyFile << Line << endl;
		} 
		MyFile.close();
	}
	cout << Green << "\n Data generated successfully in the text file : "<< FileName << ResetDefault;
}

void ShowGenerateDataScreen()
{
	system("cls");
	if (AllColumns.size() == 0)
	{
		cout << "You have No Columns , yet";
		PressAnyKey("Main menu ...");
		ShowMainMenu();
	}
	

	PrepareValues();
	
		long long NumberOfRows;
		system("cls");
		cout << "\n How many number of rows do you want ? : ";
		do
		{
			NumberOfRows = ReadNumber();
		} while (NumberOfRows <= 0);
		GenerateEntireData(NumberOfRows);
		cout << "\n Press any key to terminate ... \n";
		system("pause>0");
	

	
	

}

void PerformSelectFeatureOption(enFeature Choice)
{
	switch (Choice)
	{

		case enFeature::EditColumn:
		{
			ShowEditCoulmnsScreen();
			PressAnyKey("Main Menu ...");
			ShowMainMenu();
			break;
		}
		

		case enFeature::GenerateData:
		{
			ShowGenerateDataScreen();
			break;
		}

		case enFeature::ShowAllColumns :
		{
			ListAllColumns();
			PressAnyKey("Main Menu ...");
			ShowMainMenu();
			break;
		}


	default:
		break;
	}
}



void ShowMainMenu()
{
	system("cls");
	cout << "\nWhat do you want to do [ 1 , 2 , 3 ] ?\n";

	enFeature Choice;
	Choice = ReadFeatureOption();

	PerformSelectFeatureOption(Choice);

}




int main()
{
	srand((unsigned)time(NULL));
	ShowMainMenu();
}
