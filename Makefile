format: 
	find . -name \*.h -print -o -name \*.c -print | xargs clang-format -i -style=file	
