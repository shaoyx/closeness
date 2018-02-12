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
name = "greedy algorithm with any approximation",
description = "remove vertex under a threshold. refer details to Bahmani"
)
public class GreedyAnyApproAlgorithm  
implements GenericGraphTool{


	private int optK; /* the size of the densest subgraph */
	private Set<Integer> optVertexSet; /* the vertex set of the densest subgraph */
	private int optEdgeSize;
	private double optDensity; /* the optimum density */
	private double errorRate;
	
	Graph graph;
	private HashMap<Integer, Integer> degreeList;
	private Set<Integer> vertexSet;
	private int edgeSize;
	private int vertexSize;

	
	@Override
	public void run(CommandLine cmd) {
	    graph = new Graph(cmd.getOptionValue("i"));
	    errorRate = Double.valueOf(cmd.getOptionValue("e"));
	    
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
				" Edge Density: "+ String.format("%.5f", optEdgeSize / optVertexSet.size() / (optVertexSet.size() - 1.0)));
	}

	@Override
	public boolean verifyParameters(CommandLine cmd) {
		if(cmd.hasOption("e") == false){
			return false;
		}
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
		
		ArrayList<Integer> removedVertexList = new ArrayList<Integer>();
		
		/* algorithm body
		 * O(V^2 + M) 
		 */
		while(vertexSet.isEmpty() == false){ /* O(V) */
			
			/* 1. get candidates for removing */
			ArrayList<Integer> candidates = new ArrayList<Integer>();
			for(Integer vid : vertexSet){ /* O(V) */
				/* the density has been doubled compared to the original paper. 
				 * prune fast than Charikar. */
				if(degreeList.get(vid) <=  (1 + errorRate) * density){
					candidates.add(vid);
				}
			}
//			
//			if(candidates.isEmpty() == true){
//				System.out.println("WARNING: the vertexSet is not empty. Size="+vertexSet.size());
//				return;
//			}
			
			/* 2. update the graph */
			for(int vid : candidates){
				vertexSet.remove(vid);
				degreeList.remove(vid); /* NOTE: must remove!!! */
				removedVertexList.add(vid);
				
				ArrayList<Integer> neighbor = graph.getNeighbors(vid);
				for(Integer nvid : neighbor){
					if(degreeList.containsKey(nvid)){
						edgeSize -= 2; /* here eliminate all the cross edges, so it should be 2 in undirected graph. */
						degreeList.put(nvid, degreeList.get(nvid) - 1);
					}
					if(vid == nvid){
						edgeSize-=2;
					}
				}
			}
			
			/* 3. update the new density */
			density = edgeSize * 1.0 / vertexSet.size();		
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
//				if(optK != optVertexSet.size()){
//					System.out.println("Erorr inconsistent.");
//				}
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
