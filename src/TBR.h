//
//  Copyright 2002-2007 Rick Desper, Olivier Gascuel
//  Copyright 2007-2014 Olivier Gascuel, Stephane Guindon, Vincent Lefort
//
//  This file is part of FastME.
//
//  FastME is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  FastME is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with FastME.  If not, see <http://www.gnu.org/licenses/>
//


#ifndef TBR_H_
#define TBR_H_

#include "SPR.h"

void assignTBRUpWeights (edge *ebottom, node *vtest, node *va, edge *back,
	node *cprev, double oldD_AB, double coeff, double **A, double **dXTop,
	double ***swapWeights, edge *etop, double *bestWeight, edge **bestSplit,
	edge **bestTop, edge **bestBottom);
void assignTBRDownWeightsUp (edge *etest, node *vtest, node *va, edge *back,
	node *cprev, double oldD_AB, double coeff, double **A, double ***swapWeights,
	double *bestWeight, edge **bestSplitEdge, edge **bestTop, edge **bestBottom);
void assignTBRDownWeightsSkew (edge *etest, node *vtest, node *va, edge *back,
	node *cprev, double oldD_AB, double coeff, double **A, double ***swapWeights,
	double *bestWeight, edge **bestSplitEdge, edge **bestTop, edge **bestBottom);
void assignTBRDownWeightsDown (edge *etest, node *vtest, node *va, edge *back,
	node *cprev, double oldD_AB, double coeff, double **A, double ***swapWeights,
	double *bestWeight, edge **bestSplitEdge, edge **bestTop, edge **bestBottom);
void calcTBRTopBottomAverage (node *vbottom, double **A, double **dXTop,
	double dXOut, edge *esplit, edge *etop, edge *eback, int UpOrDown);
void calcTBRaverages (tree *T, edge *esplit, double **A, double **dXTop);
void TBR (tree *T, double **A, double **D, int *count, FILE *statfile);
void TBRswitch (tree *T, edge *es, edge *et, edge *eb);

#endif /*TBR_H_*/

