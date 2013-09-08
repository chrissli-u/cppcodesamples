#include "stdafx.h"


using namespace std;


#define SEPARATOR ";"
#define INPUTLINEBUFFERLENGTH 1024
#define RECORDSNUMBERTOACCEPT 16


class Student
{
public:
	Student(int rollNumber, int score):
		_rollNumber(rollNumber),
		_score(score)
	{
		if (!Validate(rollNumber, score))
		{
			throw std::runtime_error("invalid data");
		}
	}

public:
	int GetScore() const { return _score; }
	int GetRollNumber() const { return _rollNumber; }

private:
	string ToString() const
	{
		stringstream objectFormatter;
		objectFormatter << _rollNumber << "-" << _score;
		
		return objectFormatter.str();
	}

	static bool Validate(int rollNumber, int score) { return true; }

private:
	friend string to_string(const Student &);

	int _rollNumber;
	int _score;
};

string to_string(const Student & student)
{
	return student.ToString();
}


int StringToNumber(const string & stringToConvert, bool &badInput)
{
	stringstream auxStream;
	
	int convertedNumber;

	auxStream << stringToConvert;
	auxStream >> convertedNumber;

	if (!(badInput = auxStream.bad()))
	{
		return convertedNumber;
	}
	else
	{
		return -1;
	}	
}

shared_ptr<Student> ProcessNextInputLine(const string & inputLineString)
{
	shared_ptr<Student> sp_resultStudent;

	do
	{
		size_t separatorPosition = inputLineString.find('-');	
		
		if (separatorPosition == string::npos)
		{
			break;
		}

		string rollNumberString(inputLineString.substr(0, separatorPosition));
		string scoreString(inputLineString.substr(separatorPosition + 1));

		bool convertError;
		int rollNumberInteger = StringToNumber(rollNumberString, convertError);
		if (convertError)
		{
			break;
		}

		int scoreInteger = StringToNumber(scoreString, convertError);
		if (convertError)
		{
			break;
		}
		
		sp_resultStudent = make_shared<Student>(rollNumberInteger, scoreInteger);
	}
	while (false);
		
	return sp_resultStudent;
}


int main()
{
	typedef std::vector<shared_ptr<Student> > StudentsPtrsCollection;
	typedef StudentsPtrsCollection::const_iterator StudentsConstIterator;
	typedef StudentsPtrsCollection::iterator StudentsIterator;
	
	StudentsPtrsCollection inputStudentsCollection;
	StudentsPtrsCollection outputStudentsCollection;

	//
	// Input
	//
	{
		int acceptedRecordNumber = 0;

		while (cin.good() && acceptedRecordNumber < RECORDSNUMBERTOACCEPT)
		{
			char lineBuffer[INPUTLINEBUFFERLENGTH];

			istream &inputStream = cin.getline(lineBuffer, sizeof(lineBuffer));

			if (inputStream.good())
			{
				shared_ptr<Student> nextStudentReaded = ProcessNextInputLine(lineBuffer);
				if (nextStudentReaded != nullptr)
				{
					inputStudentsCollection.push_back(nextStudentReaded);

					++acceptedRecordNumber;
				}
			}
		}
	}

	//
	// Processing
	//
	{
		if (!inputStudentsCollection.empty())
		{
			std::sort(inputStudentsCollection.begin(), inputStudentsCollection.end(), [](const shared_ptr<Student> & s1, const shared_ptr<Student> & s2) -> bool {
				return (s1->GetRollNumber() < s2->GetRollNumber() ) || (s1->GetRollNumber() == s2->GetRollNumber() && (s1->GetScore() < s2->GetScore()));
			});

			StudentsConstIterator	it_previousStudent = inputStudentsCollection.begin(), 
									it_currentStudent = inputStudentsCollection.begin(), 
									it_studentsEnd = inputStudentsCollection.end();
			
			for (++it_currentStudent ; it_currentStudent != it_studentsEnd; ++it_currentStudent)
			{
				const Student & currentSudent = *(*it_currentStudent);
				const Student & previousStudent = *(*it_previousStudent);

				if ((currentSudent.GetRollNumber() != previousStudent.GetRollNumber()))
				{
					outputStudentsCollection.push_back(*it_previousStudent);
				}

				if (it_currentStudent != it_studentsEnd)
				{
					it_previousStudent = it_currentStudent;
				}				
			}
			outputStudentsCollection.push_back(*it_previousStudent);

			std::sort(outputStudentsCollection.begin(), outputStudentsCollection.end(), [](const shared_ptr<Student> & s1, const shared_ptr<Student> & s2) -> bool {
				return s1->GetScore() > s2->GetScore();
			});
		}
	}

	//
	// Output
	//
	{
		for (const auto & p_currentStudent : outputStudentsCollection)
		{
			cout << to_string(*p_currentStudent) << std::endl;
		}
	}
}

