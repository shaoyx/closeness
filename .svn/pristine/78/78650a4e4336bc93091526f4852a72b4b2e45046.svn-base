package com.graphtools.simrank;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.PriorityQueue;

import com.graphtools.utils.GraphAnalyticTool;
import com.graphtools.utils.Random;

@GraphAnalyticTool(
name = "SimRankGSasIndex",
description = "Compute SimRank based on graph sampling. The sampled graph are treated as index and sample random walks of query vertex" +
		"to compute an approximating simrank."
)
public class SimRankGSasIndex {
	
	private int [] sampledEdges;
	private int [] indegrees;
	private double [] simrankValues;
	HashMap<Integer, ArrayList<Integer>> visitedTimestamp = new HashMap<Integer, ArrayList<Integer>>();

	private HashMap<Integer, Integer> vertexMap;
	/* new id ==> original id */
	private HashMap<Integer, Integer> rvertexMap;
	/* use the new id and store the reversed graph. */
	private HashMap<Integer, ArrayList<Integer>> edgeList;
	
	private int sampleNum = 100;
	private int querySampleNum = 100;
	private int MAX_STEPS = 11;
	private int qv = -1;
	private int k;
	private double decayFactor = 0.6;
	
	public SimRankGSasIndex(SimRankUniverse sru){
		this.vertexMap = sru.vertexMap;
		this.rvertexMap = sru.rvertexMap;
		this.edgeList = sru.edgeList;
		this.sampleNum = sru.sampleNum;
		this.querySampleNum = sru.querySampleNum;
		this.MAX_STEPS = sru.MAX_STEPS;
		this.qv = sru.qv;
		this.k = sru.k;
		this.decayFactor =sru.decayFactor;
	}
	
	
	public void topksimrank(){
		sampledEdges = new int[vertexMap.size()];
		simrankValues = new double[vertexMap.size()];
		indegrees = new int[vertexMap.size()];
		for(int vid = 0; vid < vertexMap.size(); vid++){
			simrankValues[vid] = 0;
		}
		simrankValues[qv] = -10;
		
		for(int i = 0; i < sampleNum; i++){
			/* 1. sample a graph as other vertices' random walks */
			for(int vid = 0; vid < vertexMap.size(); vid++){
				indegrees[vid] = 0;
			}
			for(int vid = 0; vid < vertexMap.size(); vid++){
				ArrayList<Integer> nb = edgeList.get(vid);
				if(nb == null || nb.size() == 0){
					sampledEdges[vid] = -1;
				}
				else{
					sampledEdges[vid] = nb.get((int)(getRandomNextInt() % nb.size()));
					indegrees[sampledEdges[vid]]++;
				}
			}
			
			for(int j = 0; j < querySampleNum; j++){
				visitedTimestamp.clear();
				/* 2. calculate a random walk for the queryVertex. */
				int curVertex = qv;
				for(int s = 1; s <= MAX_STEPS; s++){
					ArrayList<Integer> nb = edgeList.get(curVertex);
					int next = -1;
					if(nb == null || nb.size() == 0){
						break;
					}
					else{
						next = nb.get((int)(getRandomNextInt() % nb.size()));
					}
					if(visitedTimestamp.get(next) == null){
						visitedTimestamp.put(next, new ArrayList<Integer>());
					}
					visitedTimestamp.get(next).add(s);
					curVertex = next;
				}
				
				
				/* 3. compute the simrank */
				computePairWiseSimRank();
			}
//			System.out.println("Sample "+i+":");
//			for(int vid = 0; vid < vertexMap.size(); vid++){
//				System.out.println("vid="+rvertexMap.get(vid)+" nb="+rvertexMap.get(sampledEdges[vid])+", smv="+simrankValues[vid]);
//			}
		}
		/* 3. get the top-k simrank */
		PriorityQueue<SimRankPair> pq = new PriorityQueue<SimRankPair>();
		for(int vid = 0; vid < vertexMap.size(); vid++){
			pq.add(new SimRankPair(vid, simrankValues[vid]));
			if(pq.size() > k)
				pq.remove(); // remove the smallest one
		}
		/* output the answer */
		int cnt = k;
//		System.out.println("(1300, " + String.format("%.9f", simrankValues[vertexMap.get(1300)] / sampleNum)+")");
//		System.out.println("(378, " + String.format("%.9f", simrankValues[vertexMap.get(378)] / sampleNum)+")");
//		System.out.println("(301, " + String.format("%.9f", simrankValues[vertexMap.get(301)] / sampleNum)+")");
		System.out.println("SimRankGS: ");
		while(pq.isEmpty() == false){
			SimRankPair srp = pq.poll();
			System.out.println(cnt+" ==> ("+ rvertexMap.get(srp.getVertex())+", " + String.format("%.9f", srp.getSimRank() / sampleNum / querySampleNum)+")");
			cnt--;
		}
	}
	
	private void computePairWiseSimRank() {
		/* 1. compute each pairwise simrank. */
		double [] factor = new double[MAX_STEPS + 1];
		factor[0] = 1.0;
		for(int i = 1; i <= MAX_STEPS; i++){
			factor[i] = factor[i-1] * decayFactor;
		}

		int [] expanded = new int[vertexMap.size()];
		for(int v = 0; v < vertexMap.size(); v++){
			expanded[v] = -(1 << 30);
		}
		
		int arrayLen = MAX_STEPS + 1;
		int [] path = new int[arrayLen]; /* record the path. using cyclic array. */
		
		/* do the real computation*/
		for(int vid = 0; vid < vertexMap.size(); vid++){
			if(expanded[vid] == -(1<<30)){
				/** NOTE: search one path and compute meet points. */
				/* initialization */
				int tail = 0;
				path[tail++] = vid;
				int lastUniqVertex = 0; /* The index of last vertex that hasn't be expanded. */
				int step = 0; /* current length of the path */
				expanded[vid] = 1;
				boolean isFull = false;

//				System.out.println("arrayLengh="+arrayLen+" tail="+tail);
				/* compute simrank */
				while(step <= lastUniqVertex + MAX_STEPS){
					int next = sampledEdges[path[((tail - 1) + arrayLen) % arrayLen]];
					if(next == -1) break;
					step++; // current lenght of the path.
					
					if(visitedTimestamp.get(next) != null){
						for(int stamp : visitedTimestamp.get(next)){
							int delta = tail - stamp;
							if(isFull == true && delta < 0) 
								delta += arrayLen; // MAX_STEPS + MAX_STEPS + 1 is the length of array.
							if(delta >= 0 && path[delta] != qv && (step - stamp) == (expanded[path[delta]] - 1)){
//								if(path[delta] == vertexMap.get(6))
//									System.out.println("s="+step+" stamp="+stamp+" start="+vertexMap.get(vid)+" vid="+path[delta]+" delta="+factor[stamp]);
								simrankValues[path[delta]] += factor[stamp];
								expanded[path[delta]] *= -1; // make sure each vertex is only meet once.
							}
						}
					}
					
					/* maintain the last unique index. */
					if(expanded[next] == -(1 << 30)){
						expanded[next] = step + 1;
						lastUniqVertex = step;
					}
					
					/* maintain the queue. */
					path[tail] = next; // record the path
					tail++;
					if(tail >= arrayLen){
						isFull = true;
						tail = 0;
					}
				}
//				for(int v = 0; v < vertexMap.size(); v++){
//					if(expanded[v] > 0)
//						expanded[v] *= -1;
//				}
			}
		}
	}

	private long getRandomNextInt(){
		long rand = Random.nextInt();
		return rand < 0 ? -rand : rand;
	}

}
