package com.graphtools.densegraph;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Set;

import org.apache.commons.cli.CommandLine;

import com.graphtools.GenericGraphTool;
import com.graphtools.utils.Graph;
import com.graphtools.utils.GraphAnalyticTool;

@GraphAnalyticTool(
name = "greedy approximation algorithm",
description = "remove the smallest degree vertex one by one. refer details to Charikar"
)
public class GreedyApproAlgorithm 
implements GenericGraphTool{

	private int optK; /* the size of the densest subgraph */
	private Set<Integer> optVertexSet; /* the vertex set of the denset subgraph */
	private int optEdgeSize;
	private double optDensity; /* the optimum density */
	
	Graph graph;
	private HashMap<Integer, Integer> degreeList;
	private Set<Integer> vertexSet;
	private int edgeSize;
	private int vertexSize;
	
	@Override
	public void run(CommandLine cmd) {
	    graph = new Graph(cmd.getOptionValue("i"));
		vertexSet = new HashSet<Integer>();
		degreeList = new HashMap<Integer, Integer>();
		
		edgeSize = graph.getEdgeSize();
		vertexSize = graph.getVertexSize();
		for(int vid : graph.getVertexSet()){
			vertexSet.add(vid);
			degreeList.put(vid, graph.getDegree(vid));
		}
		
		runGreedyAlgorithm();
		
		System.out.println("Optimum size: "+ optK+", Denstiy: " + String.format("%.5f", optDensity/2)+
				" EdgeSize="+optEdgeSize+
				" vsize="+optVertexSet.size()+
				" Edge Density: "+ String.format("%.5f",optEdgeSize / optVertexSet.size() / (optVertexSet.size() - 1.0)));
	}

	@Override
	public boolean verifyParameters(CommandLine cmd) {
		return true;
	}
	
	private void runGreedyAlgorithm() {
		double density = edgeSize * 1.0 / vertexSize;
		optDensity = density;
		optK = vertexSize;
		optEdgeSize = edgeSize;
		optVertexSet = new HashSet<Integer>();
		for(Integer vid : vertexSet){
			optVertexSet.add(vid);
		}
		
		/* only for debug */
//		if(variantCheck() == false){
//			System.out.println("Invariant Failed: curreut vertexSize="+vertexSet.size());
//		}
		
		ArrayList<Integer> removedVertexList = new ArrayList<Integer>();
		
		/* algorithm body
		 * O(V^2 + M) 
		 */
		while(vertexSet.isEmpty() == false){ /* O(V) */
			/* 1. select the smallest degree vertex */
			int selectVertex = -1; /* choose the first one when tie occurs. */
			for(Integer vid : vertexSet){ /* O(V) */
				if(selectVertex == -1 || degreeList.get(vid) < degreeList.get(selectVertex)){
					selectVertex  = vid;
				}
			}
			
			/* 2. update the graph */
			vertexSet.remove(selectVertex);
			degreeList.remove(selectVertex); /* NOTE: must remove!!! */
			removedVertexList.add(selectVertex);
			
			ArrayList<Integer> neighbor = graph.getNeighbors(selectVertex);
			for(Integer vid : neighbor){
				if(degreeList.containsKey(vid)){
					edgeSize -= 2; /* here eliminate all the cross edges, so it should be 2 in undirected graph. */
					degreeList.put(vid, degreeList.get(vid) - 1);
				}
				
				/* loop case!!!! */
				if(vid == selectVertex){
					edgeSize -= 2;
				}
			}
			
			/* 3. update the new density */
			density = edgeSize * 1.0 / vertexSet.size();
			
			/* only for debug */
//			if(variantCheck() == false){
//				System.out.println("Invariant Failed: curreut vertexSize="+vertexSet.size());
//			}
			
			if(density > optDensity){
				optDensity = density;
				optK = vertexSet.size();
				optEdgeSize = edgeSize;
				for(int vid : removedVertexList){
					optVertexSet.remove(vid);
				}
				removedVertexList.clear();
			}
		}
	}
	
	private boolean variantCheck(){
		int degreeSum = 0;
		for(int vid : vertexSet){
			degreeSum += degreeList.get(vid);
		}
		return edgeSize == degreeSum;
	}

}
