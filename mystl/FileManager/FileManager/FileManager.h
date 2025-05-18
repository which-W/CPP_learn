
#pragma once

#include <iostream>
#include <cstdio>

struct FileDeleter
{
	void operator()(FILE* file) {
		if (file)
		{
			fclose(file);
		}
	}
};
