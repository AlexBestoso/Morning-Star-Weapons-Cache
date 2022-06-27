/*
 *  This class is designed to make managing data permutations easier to 
 *  control by abstracting the concepts for use elsewhere.
 *  */
class Permuter{
	private:
	public:
		/*
		 * The following pointers must use the mapper size
		 * */
		// contains increment mappings for the core string.
		size_t _mapperSize = -1;
		const int MAX_MAPPER_SIZE = 20;
		int _mapper[20] = {0};
		bool _mapperFrozen[20] = {false};
		int _mapperIncOrder[20] = {0};
		string _validCharsArray[20] = {""};
		/* End size restriction. */

		int startingBruteLength = 1;
		int maxBruteLength = 6;

		bool _maxIncrement = false;
		
		string _validChars = "";
		string _startingPattern = "";

		Permuter(){}
		Permuter(string validChars, string startingPattern, size_t mapperSize){
			_validChars = validChars;
			_startingPattern = startingPattern;
			_mapperSize = mapperSize;
			defaultMapper();
			setIncrementMap(validChars);
		}

		// find the increment values for each provided value and swap their values.
		void swapIncrementOrders(int a, int b){
			if(_mapperIncOrder == NULL || (a > _mapperSize || a < 0) || (b > _mapperSize || b < 0) || _mapperSize <= 0)
				return;
			int A = _mapperIncOrder[a];
			int B = _mapperIncOrder[b];
			_mapperIncOrder[a] = B;
			_mapperIncOrder[b] = A;
		}

		void setRoundValidCharMap(int i, string validCharMapping){
			if(i > _mapperSize || i <= 0 || _mapperSize <= 0 || _validCharsArray == NULL)
				return;
			_validCharsArray[i] = validCharMapping;
		}

		void initValidCharMap(void){
			if(_validChars == ""){
				fprintf(stderr, "[E] Make sure both _validChars and _startingPattern are set, Mr. Programmer.\n");
				return;
			}
			setIncrementMap(_validChars);
		}

		void setIncrementMap(string validChars){
			_validChars = validChars;
        		if(_mapper == NULL || _mapperSize > validChars.length()){
        		        return;
       	 		}
			if(_validCharsArray == NULL)
				return;

			for(int i=0; i<_mapperSize; i++){
				_validCharsArray[i] = validChars;
			}
		}
		// gets the ith value of the current mapped orientation.
		char getValueByI(int i){
			if(_mapper == NULL || _validChars.length() <= 0 || i >= _validChars.length())
				return 0x00;
			return _validChars[_mapper[i]];
		}

		// Increments the starting char string according to current configuration
		void Increment(void){
			if(_maxIncrement){
				fprintf(stderr, "[W] Permuter can't increment any more.\n");
				return;
			}
			bool inc = true;
			for(int i=0; i<_mapperSize; i++){
				if(_mapperFrozen[i] == true)
					continue;
        		        if(inc)
        		                _mapper[_mapperIncOrder[i]]++;
        		        else
        		                break;

        		        if(_mapper[_mapperIncOrder[i]] >= _validChars.length() ){
        		                _mapper[_mapperIncOrder[i]] = 0;
        		                inc = true;
        		        }else{
        		                inc = false;
        		        }
        		}

        		if(!inc){
        		        _maxIncrement = false;
			}else{
                		_maxIncrement = true;
			}
		}
		void resetMapper(void){
			if(_mapper == NULL)
				return;
                      	_maxIncrement = false;
			memset(_mapper, 0, _mapperSize);
		}
		void defaultMapper(void){
			for(int i=0; i<MAX_MAPPER_SIZE; i++){
				_mapperIncOrder[i] = i;	
				_validCharsArray[i] = _validChars;
				_mapperFrozen[i] = false;
			}
		}
};
