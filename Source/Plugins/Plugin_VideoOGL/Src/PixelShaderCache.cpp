// Copyright (C) 2003 Dolphin Project.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 2.0.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License 2.0 for more details.

// A copy of the GPL 2.0 should have been included with the program.
// If not, see http://www.gnu.org/licenses/

// Official Git repository and contact information can be found at
// http://code.google.com/p/dolphin-emu/

#include "Globals.h"

#include "GLUtil.h"

#include <cmath>

#include "Statistics.h"
#include "VideoConfig.h"
#include "ImageWrite.h"
#include "Common.h"
#include "Render.h"
#include "VertexShaderGen.h"
#include "ProgramShaderCache.h"
#include "PixelShaderManager.h"
#include "OnScreenDisplay.h"
#include "StringUtil.h"
#include "FileUtil.h"
#include "Debugger.h"

namespace OGL
{

void SetPSConstant4fvByName(const char * name, unsigned int offset, const float *f, const unsigned int count = 1)
{
	ProgramShaderCache::PCacheEntry tmp = ProgramShaderCache::GetShaderProgram();
	for (int a = 0; a < NUM_UNIFORMS; ++a)
	{
		if (!strcmp(name, UniformNames[a]))
		{
			if (tmp.shader.UniformLocations[a] == -1)
				return;
			else
			{
				glUniform4fv(tmp.shader.UniformLocations[a] + offset, count, f);
				return;
			}
		}
	}
}

// Renderer functions
void Renderer::SetPSConstant4f(unsigned int const_number, float f1, float f2, float f3, float f4)
{
	float const f[4] = {f1, f2, f3, f4};

	if (g_ActiveConfig.backend_info.bSupportsGLSLUBO)
	{
		ProgramShaderCache::SetMultiPSConstant4fv(const_number, f, 1);
		return;
	}
	for (unsigned int a = 0; a < 10; ++a)
	{
		if (const_number >= PSVar_Loc[a].reg && const_number < (PSVar_Loc[a].reg + PSVar_Loc[a].size))
		{
			unsigned int offset = const_number - PSVar_Loc[a].reg;
			SetPSConstant4fvByName(PSVar_Loc[a].name, offset, f);
			return;
		}
	}
}

void Renderer::SetPSConstant4fv(unsigned int const_number, const float *f)
{
	if (g_ActiveConfig.backend_info.bSupportsGLSLUBO)
	{
		ProgramShaderCache::SetMultiPSConstant4fv(const_number, f, 1);
		return;
	}
	for (unsigned int a = 0; a < 10; ++a)
	{
		if (const_number >= PSVar_Loc[a].reg && const_number < (PSVar_Loc[a].reg + PSVar_Loc[a].size))
		{
			unsigned int offset = const_number - PSVar_Loc[a].reg;
			SetPSConstant4fvByName(PSVar_Loc[a].name, offset, f);
			return;
		}
	}
}

void Renderer::SetMultiPSConstant4fv(unsigned int const_number, unsigned int count, const float *f)
{
	if (g_ActiveConfig.backend_info.bSupportsGLSLUBO)
	{
		ProgramShaderCache::SetMultiPSConstant4fv(const_number, f, count);
		return;
	}
	for (unsigned int a = 0; a < 10; ++a)
	{
		if (const_number >= PSVar_Loc[a].reg && const_number < (PSVar_Loc[a].reg + PSVar_Loc[a].size))
		{
			unsigned int offset = const_number - PSVar_Loc[a].reg;
			SetPSConstant4fvByName(PSVar_Loc[a].name, offset, f, count);
			return;
		}
	}
}
}  // namespace OGL
