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


#include "traverse.h"

/* findBottomLeft searches by bottom down in the tree and to the left */

/*********************************************************/

edge *findBottomLeft (edge *e)
{
	edge *f;
	f = e;

	while (NULL != f->head->leftEdge)
		f = f->head->leftEdge;

	return (f);
}

/*********************************************************/

edge *findBottomRight (edge *e)
{
	edge *f;
	f = e;

	while (NULL != f->head->rightEdge)
		f = f->head->rightEdge;

	return(f);
}

/*********************************************************/

edge *moveRight (edge *e)
{
	edge *f;
	f = e->tail->rightEdge;		/* this step moves from a left-oriented
								 * edge to a right-oriented edge */
	if (NULL != f)
		f = findBottomLeft (f);

	return (f);
}

/*********************************************************/

edge *moveMiddle (edge *e)
{
	edge *f;
	f = e->tail->middleEdge;

	if (NULL == f)
		return (e->tail->parentEdge);

	else
		f = findBottomLeft (f);

	return (f);
}

/*********************************************************/

edge *moveRightFirstMiddle (edge *e)
{
	edge *f;
	f = e->tail->middleEdge;

	if (NULL == f)
		return(e->tail->parentEdge);

	else
		f = findBottomRight (f);

	return (f);
}

/*********************************************************/

edge *moveLeft (edge *e)
{
	edge *f;
	f = e->tail->leftEdge;		/* this step moves from a left-oriented
								 * edge to a right-oriented edge */
	if (NULL != f)
		f = findBottomRight (f);

	return (f);
}

/* depthFirstTraverse returns the edge f which is least in T according
 * to the depth-first order, but which is later than e in the search
 * pattern.  If e is null, f is the least edge of T */

/*********************************************************/

edge *depthFirstTraverse (tree *T, edge *e)
{
	edge *f;

	if (NULL == e)
	{
		f = T->root->leftEdge;
		if (NULL != f)
			f = findBottomLeft(f);

		/* f is the first edge of this search pattern */
	}
	else	/* e is non-null */
	{
		if (e->tail->leftEdge == e)		/* if e is a left-oriented edge,
										 * we skip the entire tree cut
										 * below e, and find least edge */
			f = moveRight (e);

		else		/* if e is a right-oriented edge,
					 * we have already looked at its
					 * sibling and everything below e, so we move up */
			f = e->tail->parentEdge;
	}

	return (f);
}

/*********************************************************/

edge *moveUpRight (edge *e)
{
	edge *f;
	f = e;

	while ((NULL != f) && ( f->tail->leftEdge != f))
		f = f->tail->parentEdge;

	/* go up the tree until f is a leftEdge */
	if (NULL == f)
		return (f);		/* triggered at end of search */

	else
		return (f->tail->rightEdge);	/* and then go right */
}

/* topFirstTraverse starts from the top of T,
 * and from there moves stepwise down, left before right
 * assumes tree has been detrifurcated */

/*********************************************************/

edge *topFirstTraverse (tree *T, edge *e)
{
	edge *f;

	if (NULL == e)
		return (T->root->leftEdge);		/* first Edge searched */

	else if (!(leaf(e->head)))
		return (e->head->leftEdge);		/* down and to the left is preferred */

	else	/* e->head is a leaf */
	{
		f = moveUpRight (e);
		return(f);
	}
}

/*********************************************************/

edge *depthRightFirstTraverse (tree *T, edge *e)
{
	edge *f;

	if (NULL == e)
	{
		f = T->root->rightEdge;
		if (NULL != f)
			f = findBottomRight(f);

		return (f);		/* this is the first edge of this search pattern */
	}
	else	/* e is non-null */
	{
		if (e->tail->rightEdge == e)		/* if e is a right-oriented edge,
											 * we skip the entire tree cut
											 * below e, and find least edge */
			f = moveLeft(e);

		else		/* if e is a right-oriented edge, we have already
					 * looked at its sibling and everything below e,
					 * so we move up*/
			f = e->tail->parentEdge;
	}

	return (f);
}

/*********************************************************/

node *findNodeFromName (tree *T, char *name)
{
	node *n = NULL;

	// Node label != name
	if (0 != strncmp (name, T->root->label, MAX_NAME_LENGTH))
	{
		n = traverseEdgeNodeName (T->root->leftEdge, name);
		if (NULL == n)
		{
			n = traverseEdgeNodeName (T->root->middleEdge, name);
		}
		if (NULL == n)
		{
			n = traverseEdgeNodeName (T->root->rightEdge, name);
		}
	}
	
	return (n);
}

/*********************************************************/

node *traverseEdgeNodeName (edge *E, char *name)
{
	node *n = NULL;
	
	if (NULL != E)
	{
		// Node label == name
		if (0 == strncmp (name, E->head->label, MAX_NAME_LENGTH))
		{
			n = E->head;
		}
		else
		{
			if (! leaf (E->head))
			{
				n = traverseEdgeNodeName (E->head->leftEdge, name);
				if (NULL == n)
					n = traverseEdgeNodeName (E->head->middleEdge, name);
				if (NULL == n)
					n = traverseEdgeNodeName (E->head->rightEdge, name);
			}
		}
	}
	
	return n;
}

/*********************************************************/

void traverseFromNode (node *n)
{
	double d = 0.0;
	
	traverseEdgeDist (n->parentEdge, n);
	
	return;
}

/*********************************************************/

void traverseEdgeDist (edge *E, node *n)
{
	node *m = NULL;
	
	if (NULL != E)
	{
		printf ("\nEdge '%s' distance %f\n", E->label, E->distance);
	//ne pas comparer des pointeurs, faut comparer des labels
		if (E->tail == n)
			m = E->head;
		else
			m = E->tail;

		printf ("\nnode '%s'\n", m->label);

		if (leaf (n))
		{
			printf ("\nLeaf '%s'\n", m->label);
		}
		else
		{
			traverseEdgeDist (n->parentEdge, m);
			traverseEdgeDist (n->middleEdge, m);
			traverseEdgeDist (n->rightEdge, m);
		}
	}
	
	return;
}

/*********************************************************/

void traverseTree (tree *T)
{
	printf ("\nsize %d weight %f\n", T->size, T->weight);
	printf ("\nRoot '%s'\n", T->root->label);
	
	traverseEdge (T->root->leftEdge);
	traverseEdge (T->root->middleEdge);
	traverseEdge (T->root->rightEdge);
	
	return;
}

/*********************************************************/

void traverseEdge (edge *E)
{
	if (NULL != E)
	{
		printf ("\nEdge '%s'\nbottomsize %d topsize %d, distance %f, totalweight %f\n", E->label, E->bottomsize, E->topsize, E->distance, E->totalweight);
	
		node *n = E->head;
	
		if (leaf (n))
		{
			printf ("\nLeaf '%s'\n", n->label);
		}
		else
		{
			traverseEdge (n->leftEdge);
			traverseEdge (n->middleEdge);
			traverseEdge (n->rightEdge);
		}
	}
	
	return;
}

