class FileSnake{
	private:
		int _fd = -1;
		string openedFile = "";
		size_t _fileSize = -1;
		
		bool _openForRead(string fileName){
			if(!fileExists(fileName))
				return false;
			_closeFile();
			_fd = open(fileName.c_str(), O_RDONLY);
			if(_fd <= 0){
				return false;
			}

			openedFile = fileName;
			return true;
		}

		bool _openForWriteAppend(string fileName){
			_closeFile();
			if(!fileExists(fileName)){
				_fd = open(fileName.c_str(), O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
				if(_fd <= 0)
					return false;
				openedFile = fileName;
				return true;
			}else{
				_fd = open(fileName.c_str(), O_APPEND | O_WRONLY);
				if(_fd <= 0)
					return false;
				openedFile = fileName;
				return true;
			}
		}
		bool _openForWriteTrunc(string fileName){
			_closeFile();
			if(!fileExists(fileName)){
				_fd = open(fileName.c_str(), O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
				if(_fd <= 0)
					return false;
				openedFile = fileName;
				return true;
			}else{
				_fd = open(fileName.c_str(), O_TRUNC | O_WRONLY);
				if(_fd <= 0)
					return false;
				openedFile = fileName;
				return true;
			}
		}

		size_t _readFile(string fileName, char *outBuffer, size_t outSize){
			size_t ret = -1;
			if(!_openForRead(fileName)){
				return -1;
			}
			
			ret = read(_fd, outBuffer, outSize);
			_closeFile();

			return ret;
		}
		size_t _writeFileTrunc(string fileName, char *inBuffer, size_t inSize){
			size_t ret = -1;
			if(!_openForWriteTrunc(fileName)){
				return -1;
			}
			ret = write(_fd, inBuffer, inSize);
			_closeFile();

			return ret;
		}
		size_t _writeFileAppend(string fileName, char *inBuffer, size_t inSize){
			size_t ret = -1;
			if(!_openForWriteAppend(fileName)){
				return -1;
			}
			ret = write(_fd, inBuffer, inSize);
			_closeFile();

			return ret;
		}

		void _closeFile(){
			close(_fd);
			_fd = -1;
		}
	public:
	size_t recentActivity = -1;
	bool dirExists(string dir){
		struct stat st;
		if(stat(dir.c_str(), &st) != 0)
			return false;
		return true;
	}
	bool makeDir(string dir){
		if(!this->dirExists(dir)){
			if(mkdir(dir.c_str(), 0700))
				return false;
		}
		return true;
	}
	bool fileExists(string file){
		struct stat st;
		if(stat(file.c_str(), &st) != 0)
			return false;
		return true;
	}

	size_t getFileSize(string fileName){
		struct stat st;
		if(stat(fileName.c_str(), &st) != 0)
			return -1;
		_fileSize = st.st_size;
		return (size_t)st.st_size;
	}

	bool writeFileTrunc(string fileName, char *inBuffer, size_t bufferSize){
		recentActivity = _writeFileTrunc(fileName, inBuffer, bufferSize);
		if(recentActivity  != bufferSize)
			return false;
		return true;
	}

	bool writeFileAppend(string fileName, char *inBuffer, size_t bufferSize){
		recentActivity = _writeFileAppend(fileName, inBuffer, bufferSize);
		if(recentActivity != bufferSize)
			return false;
		return true;
	}

	bool removeFile(string fileName){
		if(unlink(fileName.c_str()) != 0)
			return false;
		return true;
	}

	bool readFile(string fileName, char *outBuffer, size_t fileSize){
		if(fileSize <= 0)
			return false;

		memset(outBuffer, 0x00, fileSize);


		recentActivity = _readFile(fileName, outBuffer, fileSize);
		if(recentActivity <= 0)	
			return false;
		
		return true;
	}
};
