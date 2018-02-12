package com.graphtools.densegraph;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.PriorityQueue;
import java.util.Set;

import org.apache.commons.cli.CommandLine;

import com.graphtools.GenericGraphTool;
import com.graphtools.utils.Graph;
import com.graphtools.utils.GraphAnalyticTool;

@GraphAnalyticTool(
name = "partition based algorithm for k-sized densest subgraph",
description = "adapt from KL algorithm"
)
public class PartitionBasedAlgorithm  
implements GenericGraphTool{

//	private double optDensity;
	private Set<Integer> optVertexSet;
	private int optEdgeSize;
	
	private int objectValue; /* edgeSize = optEdgeSize + objectValue */
	
	private int thredshold;
	private Graph graph;
	
	@Override
	public void run(CommandLine cmd) {
		thredshold = Integer.valueOf(cmd.getOptionValue("k"));
		graph = new Graph(cmd.getOptionValue("i"));
		
		optVertexSet = new HashSet<Integer>();
		optEdgeSize = graph.getEdgeSize();
		objectValue = 0;
		
		for(int vid : graph.getVertexSet()){
			optVertexSet.add(vid);
		}
		
		runModifiedKLAlgorithm();
//		printOptVertexSet();
		System.out.println("Optimun Edge Size= "+optEdgeSize+ " optVertexSize="+optVertexSet.size()+
				String.format(" Density= %.5f", optEdgeSize * .5 / optVertexSet.size())+
				String.format(" EdgeDensity= %.5f",optEdgeSize/(optVertexSet.size())/(optVertexSet.size()-1.0)));
	}

	@Override
	public boolean verifyParameters(CommandLine cmd) {
		if(cmd.hasOption("k") == false){
			return false;
		}
		return true;
	}

	private void runModifiedKLAlgorithm() {
		/* 1. greedy generating (k,v-k) partition */
		HashMap<Integer, Integer> degreeList = new HashMap<Integer, Integer>();
		Set<Integer> counterPart = new HashSet<Integer>();
		for(int vid : optVertexSet){
			degreeList.put(vid, graph.getDegree(vid));
		}
		while(optVertexSet.size() > thredshold){
			int removeId = -1;
			for(int vid : optVertexSet){
				if(removeId == -1 || degreeList.get(removeId) > degreeList.get(vid)){
					removeId = vid;
				}
			}
			
			degreeList.remove(removeId);
			counterPart.add(removeId);
			optVertexSet.remove(removeId);
			
			for(int vid : graph.getNeighbors(removeId)){
				if(degreeList.containsKey(vid)){
					degreeList.put(vid, degreeList.get(vid)-1);
					optEdgeSize -= 2;
					objectValue += 2;
				}
				if(vid == removeId){
					optEdgeSize -= 2;
					objectValue += 2;
				}
			}
		}	

		printStatistic(optVertexSet, counterPart);
		/* 2. refinement with KL algorithm */
//		KernighanLinRefinement(optVertexSet, counterPart);
		KernighanLinRefinementWithPrune(optVertexSet, counterPart);
		printStatistic(optVertexSet, counterPart);
	}

	private void printStatistic(Set<Integer> optVertexSet,
			Set<Integer> counterPart) {
		int interEdge = 0;
		for(int vid : graph.getVertexSet()){
			if(optVertexSet.contains(vid)){
				for(int nvid : graph.getNeighbors(vid)){
					if(optVertexSet.contains(nvid)){
						interEdge++;
					}
					if(nvid == vid){
						interEdge++;
					}
				}
			}
		}
		System.out.println("totalEdge="+graph.getEdgeSize() +" objectValue="+objectValue);
		System.out.println("optEdgeSize="+optEdgeSize+ " interEdge="+interEdge+" optVertexSize="+optVertexSet.size()+
		String.format(" Density = %.5f",  optEdgeSize*.5/optVertexSet.size())+
		String.format(" EdgeDensity= %.5f",optEdgeSize*1.0/(optVertexSet.size())/(optVertexSet.size()-1.0)));
	}

	private void KernighanLinRefinement(Set<Integer> optVertexSet,
			Set<Integer> counterPart) {
		
		boolean isImproved = false;
		
		/* main body for refinement */
		do{
			/* 1. compute weights */
			HashMap<Integer, Integer> weights = new HashMap<Integer, Integer>();
			for(int vid : graph.getVertexSet()){
				int degreeLeft = 0;
				for(int nvid : graph.getNeighbors(vid)){
					if(optVertexSet.contains(nvid)){
						degreeLeft += 2; /* multiply 2 implicitly, and consider loop already. */
					}
					if(nvid == vid && counterPart.contains(nvid)){
						degreeLeft += 2; /* loop in left */
					}
				}
				weights.put(vid, degreeLeft);
			}
			/* 2. construct two unswap vertex set */
			ArrayList<Integer> swapLeft = new ArrayList<Integer>();
			ArrayList<Integer> swapRight = new ArrayList<Integer>();
			int swapPoint = -1;
			int gains = 0;
			int previousGains = 0;
			
			HashSet<Integer> unswapLeft  = new HashSet<Integer>();
			HashSet<Integer> unswapRight = new HashSet<Integer>();
			for(int vid : optVertexSet){
				unswapLeft.add(vid);
			}
			for(int vid : counterPart){
				unswapRight.add(vid);
			}
			
			isImproved = false;
			/* 1. pick a swap pair */
			/* brute force first */
			while(unswapLeft.size() > 0 && unswapRight.size() > 0){
				int bestGainForSinglePair = Integer.MAX_VALUE;
				int bestVidLeft = -1, bestVidRight = -1;
				/* can parallel here. */
				for(int vidLeft : unswapLeft){
					for(int vidRight : unswapRight){
						int weight = (graph.getNeighbors(vidLeft).contains(vidRight)) ? 2 : 0; /* check edge (vidLeft, vidRight) */
//						System.out.println("\tPair<"+vidLeft+","+vidRight+">, weight="
//						+(weight + weights.get(vidLeft) - weights.get(vidRight))+" preBestWeight="+bestGainForSinglePair);
						if(weight + weights.get(vidLeft) - weights.get(vidRight) < bestGainForSinglePair){
							bestVidLeft = vidLeft;
							bestVidRight = vidRight;
							bestGainForSinglePair = weight + weights.get(vidLeft) - weights.get(vidRight);
						}
					}
				}
				
				swapLeft.add(bestVidLeft);
				swapRight.add(bestVidRight);
				previousGains += bestGainForSinglePair;
//				System.out.println("iteration: "+ swapLeft.size()+" gains: "+previousGains+" Exchange: left="+ bestVidLeft+" right="+bestVidRight);
				if(gains > previousGains){ /* smaller is better. */
//					System.out.println("gains: "+previousGains+" Exchange: left="+ bestVidLeft+" right="+bestVidRight);
					gains = previousGains;
					swapPoint = swapLeft.size();
					isImproved = true;
				}
				unswapLeft.remove(bestVidLeft);
				unswapRight.remove(bestVidRight);
				/*update weights here */
				for(int vid : graph.getNeighbors(bestVidLeft)){
					weights.put(vid, weights.get(vid)-2);
				}
				for(int vid : graph.getNeighbors(bestVidRight)){
					weights.put(vid, weights.get(vid)+2);
				}
			}
			
			/*2. update weights and two partitions */
			if(isImproved){
				for(int i = 0; i < swapPoint; i++){
					int left = swapLeft.get(i);
					int right = swapRight.get(i);
					
					optVertexSet.add(right);
					optVertexSet.remove(left);
					counterPart.add(left);
					counterPart.remove(right);
					objectValue += gains;
				}
			}
		}while(isImproved);
		optEdgeSize = graph.getEdgeSize() - objectValue;
	}
	
	private void KernighanLinRefinementWithPrune(Set<Integer> optVertexSet,
			Set<Integer> counterPart) {
		
		boolean isImproved = false;
		
		/* main body for refinement */
		do{
			ArrayList<Integer> swapLeft = new ArrayList<Integer>();
			ArrayList<Integer> swapRight = new ArrayList<Integer>();
			int swapPoint = -1;
			int gains = 0;
			int previousGains = 0;
			
			/* 1. compute weights */
			HashMap<Integer, Integer> weights = recomputeWeights(optVertexSet, counterPart);
			
			/* 2. construct two unswap vertex set */
			HashSet<Integer> unswapLeft  = new HashSet<Integer>();
			HashSet<Integer> unswapRight = new HashSet<Integer>();
			for(int vid : optVertexSet){
				unswapLeft.add(vid);
			}
			for(int vid : counterPart){
				unswapRight.add(vid);
			}
			
			isImproved = false;
			
			/* 3. iteratively pick a swap pair */
			/* brute force first */
			while(unswapLeft.size() > 0 && unswapRight.size() > 0){

				int maxWeightRight = Integer.MIN_VALUE;
				int minWeightLeft = Integer.MAX_VALUE;
					
				ArrayList<Integer> leftSide = new ArrayList<Integer>();
				ArrayList<Integer> rightSide = new ArrayList<Integer>();
				
				/* prune strategy for the range [0,2], due to the edge connectivity. */
				for(int vid : unswapLeft){
					if(weights.get(vid) < minWeightLeft) 
						minWeightLeft = weights.get(vid);
				}
				for(int vid : unswapLeft){
					if(weights.get(vid) < minWeightLeft + 3){
						leftSide.add(vid);
					}
				}
				
				for(int vid : unswapRight){
					if(weights.get(vid) > maxWeightRight)
						maxWeightRight = weights.get(vid);
				}
				for(int vid : unswapRight){
					if(weights.get(vid) > maxWeightRight - 3){
						rightSide.add(vid);
					}
				}
				
				int bestGainForSinglePair = Integer.MAX_VALUE;
				int bestVidLeft = -1, bestVidRight = -1;
				/* can parallel here. */
				for(int vidLeft : leftSide){
					for(int vidRight : rightSide){
						int weight = (graph.getNeighbors(vidLeft).contains(vidRight)) ? 2 : 0; /* check edge (vidLeft, vidRight) */
//						System.out.println("\tPair<"+vidLeft+","+vidRight+">, weight="
//						+(weight + weights.get(vidLeft) - weights.get(vidRight))+" preBestWeight="+bestGainForSinglePair);
						if(weight + weights.get(vidLeft) - weights.get(vidRight) < bestGainForSinglePair){
							bestVidLeft = vidLeft;
							bestVidRight = vidRight;
							bestGainForSinglePair = weight + weights.get(vidLeft) - weights.get(vidRight);
						}
					}
				}
				
				swapLeft.add(bestVidLeft);
				swapRight.add(bestVidRight);
				previousGains += bestGainForSinglePair;
//				System.out.println("iteration: "+ swapLeft.size()+" gains: "+previousGains+" Exchange: left="+ bestVidLeft+" right="+bestVidRight);
				if(gains > previousGains){ /* smaller is better. */
//					System.out.println("gains: "+previousGains+" Exchange: left="+ bestVidLeft+" right="+bestVidRight);
					gains = previousGains;
					swapPoint = swapLeft.size();
					isImproved = true;
				}
				unswapLeft.remove(bestVidLeft);
				unswapRight.remove(bestVidRight);
				/*update weights here */
				for(int vid : graph.getNeighbors(bestVidLeft)){
					weights.put(vid, weights.get(vid)-2);
				}
				for(int vid : graph.getNeighbors(bestVidRight)){
					weights.put(vid, weights.get(vid)+2);
				}
			}
			
			/*2. update weights and two partitions */
			if(isImproved){
//				System.out.println("gains: " + gains + " changeSetSize=" + swapPoint);
				for(int i = 0; i < swapPoint; i++){
					int left = swapLeft.get(i);
					int right = swapRight.get(i);
					
					optVertexSet.add(right);
					optVertexSet.remove(left);
					counterPart.add(left);
					counterPart.remove(right);
				}
				objectValue += gains;
			}
		}while(isImproved);
		optEdgeSize = graph.getEdgeSize() - objectValue;
	}



	/* for debug*/
	private void printOptVertexSet() {
		for(int vid : optVertexSet){
			System.out.print(vid+" ");
		}
		System.out.println();
	}
	
	private void KernighanLinRefinementSorted(Set<Integer> optVertexSet,
			Set<Integer> counterPart) {
		
		boolean isImproved = false;
		
		/* main body for refinement */
		do{
			/* 1. compute weights */
			HashMap<Integer, VertexProperty> weights = new HashMap<Integer, VertexProperty>();
			for(int vid : graph.getVertexSet()){
				int degreeLeft = 0;
				for(int nvid : graph.getNeighbors(vid)){
					if(optVertexSet.contains(nvid)){
						degreeLeft += 2; /* multiply 2 implicity, and consider loop already. */
					}
					if(nvid == vid && counterPart.contains(nvid)){
						degreeLeft += 2; /* loop in left */
					}
				}
				weights.put(vid, new VertexProperty(vid, degreeLeft));
			}
			
			/* 2. construct unswap vertex set */
			ArrayList<Integer> swapLeft = new ArrayList<Integer>();
			ArrayList<Integer> swapRight = new ArrayList<Integer>();
			int swapPoint = -1;
			int gains = 0;
			int previousGains = 0;
			
			PriorityQueue<VertexProperty> unswapLeft  = new PriorityQueue<VertexProperty>(optVertexSet.size());
			PriorityQueue<VertexProperty> unswapRight = new PriorityQueue<VertexProperty>(counterPart.size(), new RightComparator());
			
			/* share the vertex weights */
			for(int vid : optVertexSet){
				unswapLeft.add(weights.get(vid));
			}
			for(int vid : counterPart){
				unswapRight.add(weights.get(vid));
			}
			
			/* ascending order according to the weight for the left part */
			//Collections.sort(unswapLeft);
			/* decreasing order according to the weight for the right part */
			//Collections.sort(unswapRight, );
			
			isImproved = false;
			int size = Math.min(unswapLeft.size(), unswapRight.size());
			int bestGainForSinglePair = Integer.MAX_VALUE;
			int bestVidLeft = -1, bestVidRight = -1;
			if(unswapLeft.size() < unswapRight.size()){
				//Find Right to Left
				for(int i = 0; i < size; i++){
					bestGainForSinglePair = Integer.MAX_VALUE;
					VertexProperty rightVP = unswapRight.poll();
					bestVidLeft = -1; bestVidRight = rightVP.getVid(); /* head is removed */
					System.out.println("value in queue: "+rightVP.getWeight()+ 
							" value in map: "+weights.get(bestVidRight).getWeight()
							+" vid: "+bestVidRight);
					for(VertexProperty vpLeft : unswapLeft){
						int vidLeft = vpLeft.getVid();
						int weight =  (graph.getNeighbors(vidLeft).contains(bestVidRight)) ? 2 : 0;
						if(weight + vpLeft.getWeight() - weights.get(bestVidRight).getWeight() < bestGainForSinglePair){
							bestVidLeft = vidLeft;
							bestGainForSinglePair = weight + vpLeft.getWeight() - weights.get(bestVidRight).getWeight();
						}
					}
					
					swapLeft.add(bestVidLeft);
					swapRight.add(bestVidRight);
					previousGains += bestGainForSinglePair;
					System.out.println("iteration: "+ swapLeft.size()+" gains: "+previousGains+" Exchange: left="+ bestVidLeft+" right="+bestVidRight);
					if(gains > previousGains){ /* smaller is better. */
//						System.out.println("gains: "+previousGains+" Exchange: left="+ bestVidLeft+" right="+bestVidRight);
						gains = previousGains;
						swapPoint = swapLeft.size();
						isImproved = true;
					}
					unswapLeft.remove(bestVidLeft);
//					unswapRight.remove(bestVidRight);
					for(int vid : graph.getNeighbors(bestVidLeft)){
						weights.get(vid).updateWeight(-2);
					}
					for(int vid : graph.getNeighbors(bestVidRight)){
						weights.get(vid).updateWeight(2);
					}
				}
			}
			else{
				//Find Left to Right
				for(int i = 0; i < size; i++){
					bestGainForSinglePair = Integer.MAX_VALUE;
					VertexProperty leftVP = unswapLeft.poll();
					bestVidLeft = leftVP.getVid(); bestVidRight = -1;
					System.out.println("value in queue: "+leftVP.getWeight()+
							" value in map "+weights.get(bestVidLeft).getWeight()
							+" vid: "+bestVidLeft);
					for(VertexProperty vpRight : unswapRight){
						int vidRight = vpRight.getVid();
						int weight =  (graph.getNeighbors(vidRight).contains(bestVidLeft)) ? 2 : 0;
						if(weight + weights.get(bestVidLeft).getWeight() - vpRight.getWeight() < bestGainForSinglePair){
							bestVidRight= vidRight;
							bestGainForSinglePair = weight + weights.get(bestVidLeft).getWeight() - vpRight.getWeight();
						}
					}
					
					swapLeft.add(bestVidLeft);
					swapRight.add(bestVidRight);
					previousGains += bestGainForSinglePair;
					System.out.println("iteration: "+ swapLeft.size()+" gains: "+previousGains+" Exchange: left="+ bestVidLeft+" right="+bestVidRight);
					if(gains > previousGains){ /* smaller is better. */
//						System.out.println("gains: "+previousGains+" Exchange: left="+ bestVidLeft+" right="+bestVidRight);
						gains = previousGains;
						swapPoint = swapLeft.size();
						isImproved = true;
					}
//					unswapLeft.remove(bestVidLeft);
					unswapRight.remove(bestVidRight);
					
					for(int vid : graph.getNeighbors(bestVidLeft)){
						weights.get(vid).updateWeight(-2);
					}
					for(int vid : graph.getNeighbors(bestVidRight)){
						weights.get(vid).updateWeight(2);
					}
				}
			}
			
			/*2. update weights and two partitions */
			if(isImproved){
				for(int i = 0; i < swapPoint; i++){
					int left = swapLeft.get(i);
					int right = swapRight.get(i);
					
					optVertexSet.add(right);
					optVertexSet.remove(left);
					counterPart.add(left);
					counterPart.remove(right);
					objectValue += gains;
				}
			}
		}while(isImproved);
		optEdgeSize = graph.getEdgeSize() - objectValue;
	}
	
	private HashMap<Integer, Integer> recomputeWeights(
			Set<Integer> optVertexSet2, Set<Integer> counterPart) {
		
		HashMap<Integer, Integer> weights = new HashMap<Integer, Integer>();
		
		for(int vid : graph.getVertexSet()){
			int degreeLeft = 0;
			for(int nvid : graph.getNeighbors(vid)){
				if(optVertexSet2.contains(nvid)){
					degreeLeft += 2; /* multiply 2 implicitly, and consider loop already. */
				}
				if(nvid == vid && counterPart.contains(nvid)){
					degreeLeft += 2; /* loop in left */
				}
			}
			weights.put(vid, degreeLeft);
		}
//		for(int vid : weights.keySet()){
//			System.out.println("vid="+vid+" weight="+weights.get(vid));
//		}
//		System.out.println();
		return weights;
	}
	
	
	private class VertexProperty 
	implements Comparable{
		private int vid;
		private int weight; /* connectivity to the left part */
		
		public VertexProperty(int vid, int degreeLeft) {
			this.vid = vid;
			weight = degreeLeft;
		}

		public int getVid(){
			return vid;
		}
		
		public void setVid(int vid){
			this.vid = vid;
		}
		
		public void setWeight(int w){
			weight = w;
		}
		
		public void updateWeight(int delta){
			weight += delta;
		}
		
		public int getWeight(){
			return weight;
		}

		@Override
		public int compareTo(Object arg0) {
			return weight - ((VertexProperty)arg0).getWeight();
		}
	}
	
	private class RightComparator 
	implements Comparator<VertexProperty>{

		@Override
		public int compare(VertexProperty arg0, VertexProperty arg1) {
			return arg1.getWeight() - arg0.getWeight();
		}
		
	}
}
