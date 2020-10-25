#include "MeshConverter.h"

#include <fstream>
#include <stdio.h>
#include <stdarg.h>

#include "obj_loader.h"

#include "Logger.h"

void WriteToFile(const char* fileName, const char* fmt, ...) {
	FILE *fileHandle;

	fileHandle=fopen(fileName, "w");
	if(fileHandle == NULL) {
		printf("Couldn't open %s\n", fileName);
		return;
	}

	va_list argptr;
	va_start(argptr, fmt);

	vfprintf(fileHandle, fmt, argptr);

	va_end(argptr);

	fclose(fileHandle);
}


namespace Crimson {
	void ConvertFromObj(const char* inputFile, const char* outputFile) {
		std::string inputSource;
		std::ifstream f(inputFile);
		if (f.is_open()) {
			std::string line;
			while (std::getline(f, line)) {
				inputSource += line + '\n';
			}
		} else {
			CR_LOG_ERROR("Unable to load input: %s\n", inputFile);
		}

		IndexedModel model = OBJModel(inputSource).ToIndexedModel();

		FILE *fileHandle;

		fileHandle=fopen(outputFile, "w");
		if(fileHandle == NULL) {
			printf("Couldn't open %s\n", outputFile);
			return;
		}

		fprintf(fileHandle, "positions = {\n\t");
		for (unsigned int i = 0; i < model.positions.size(); i++) {
			fprintf(fileHandle, "%g,%g,%g,", model.positions[i].x, model.positions[i].y, model.positions[i].z);
		}
		fprintf(fileHandle, "\n}\n");

		fprintf(fileHandle, "normals = {\n\t");
		for (unsigned int i = 0; i < model.normals.size(); i++) {
			fprintf(fileHandle, "%g,%g,%g,", model.normals[i].x, model.normals[i].y, model.normals[i].z);
		}
		fprintf(fileHandle, "\n}\n");

		fprintf(fileHandle, "texCoords = {\n\t");
		for (unsigned int i = 0; i < model.texCoords.size(); i++) {
			fprintf(fileHandle, "%g,%g,", model.texCoords[i].x, model.texCoords[i].y);
		}
		fprintf(fileHandle, "\n}\n");

		fprintf(fileHandle, "indices = {\n\t");
		for (unsigned int i = 0; i < model.indices.size(); i++) {
			fprintf(fileHandle, "%d, ", model.indices[i]);
		}
		fprintf(fileHandle, "\n}\n");

		fclose(fileHandle);
	}
}
