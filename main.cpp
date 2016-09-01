#include <QCoreApplication>
#include <iostream>
#include <regex>
#include <functional>
#include <sstream>
#include <algorithm>
#include <QDebug>

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	
	std::string countElementsInSequence;
	
	std::regex reg("[+]{0,1}[1-9][0-9]{0,17}");
	std::regex regForExp("[-+]?[0-9]*.?[0-9]+([eE][-+]?[0-9]+)?");
	
	std::string input;
    std::getline( std::cin, input );
    
    auto split = [](const std::string &s, char delim, std::vector<std::string> &elems) {
        std::stringstream ss;
        ss.str(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
	        if( item != "" )
            elems.push_back(item);
        }
    };
	
	std::cout << input << std::endl;
	std::vector<std::string> elems;
    split( input, ' ', elems );
	
	if( elems.size() > 0 )
	{
		countElementsInSequence = elems.at( 0 );
		elems.erase( elems.begin() );
	}
	
	if( std::regex_match( countElementsInSequence.begin(), countElementsInSequence.end(), reg ) )
	{
		countElementsInSequence.erase( std::remove_if(countElementsInSequence.begin(), 
		                                              countElementsInSequence.end(), 
		                                              [&](const char &c){ return ( (c == '+') || (c == '\n') ); }), 
		                               countElementsInSequence.end()  );
		
		int count = std::stoi( countElementsInSequence );
		int counter( count - 1 );
		
		std::vector<long double> inputValues;
		inputValues.reserve( (std::size_t)count );
		
		for( const auto &str : elems )
		{
			std::string value = str;
			std::cout << "---" << value << "---" << std::endl;
			value.erase( std::remove_if(value.begin(), 
			                            value.end(), 
			                            [&](const char &c){ return ( (c == '\n') ); }), 
			             value.end()  );
			
			try
			{
				long double dValue = std::stold( value );
				
				if( std::regex_match( value.begin(), value.end(), regForExp ) == false )
				{
					std::stringstream ss;
					ss << dValue;
					if( ss.str() != value )
						break;
				}
				
				inputValues.push_back( dValue );
			}
			catch(std::invalid_argument)
			{
				break;
			}
			
			--counter;
			if( counter < 0 )
				break;
		}
		
		//10 test complete counter > 0 not work
		if( counter >= 0 || count <= 2 )
			std::cout << "[error]" << std::endl;
		else
		{
			int index( 0 );
			int length( 0 );
			int tempLength( 0 );
			int tempIndex( 0 );
			
			for( size_t i = 1; i < inputValues.size(); ++i )
			{
				auto first = inputValues.at( i - 1 );
				auto second = inputValues.at( i );
				
				if( second >= first )
				{
					if( tempLength == 0 )
					{
						tempIndex = i;
						tempLength = 2;
					}
					else
						++tempLength;
				}
				else if( tempLength > 0 )
				{
					if( tempLength > length )
					{
						length = tempLength;
						index = tempIndex;
					}
					
					tempLength = 0;
					tempIndex = 0;
				}
			}
			
			if( length < tempLength )
			{
				length = tempLength;
				index = tempIndex;
			}
			
			if( length > 0 )
				std::cout << length << " " << index << std::endl;
			else
				std::cout << 0 << std::endl;
		}
	}
	else
		std::cout << "[error]" << std::endl;
	
	return 0;
}
