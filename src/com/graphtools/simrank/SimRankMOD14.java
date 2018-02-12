package com.graphtools.simrank;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.PriorityQueue;

import com.graphtools.utils.GraphAnalyticTool;
import com.graphtools.utils.MinHeap;
import com.graphtools.utils.Random;

@GraphAnalyticTool(
name = "SimRankMOD14",
description = "Sampling solution from SIGMOD 2014 \"Scalable Similarity Search for SimRank\" without pruning bound."
)
public class SimRankMOD14 {

	private HashMap<Integer, Integer> vertexMap;
	/* new id ==> original id */
	private HashMap<Integer, Integer> rvertexMap;
	/* use the new id and store the reversed graph. */
	private HashMap<Integer, ArrayList<Integer>> edgeList;
	
	private int sampleNum = 100;
	private int MAX_STEPS = 11;
	private int qv = -1;
	private int k;
	private double decayFactor = 0.6;
	
	public SimRankMOD14(SimRankUniverse sru){
		this.vertexMap = sru.vertexMap;
		this.rvertexMap = sru.rvertexMap;
		this.edgeList = sru.edgeList;
		this.sampleNum = sru.sampleNum;
		this.MAX_STEPS = sru.MAX_STEPS;
		this.qv = sru.qv;
		this.k = sru.k;
		this.decayFactor = sru.decayFactor;
	}
	
	public void topksimrank() {
		PriorityQueue<SimRankPair> pq = new PriorityQueue<SimRankPair>();
		
		for(int otherv : vertexMap.values()){ // enumerate new id here!!!!
			if(otherv == qv) continue; // do not recompute qv.
			/* calculate simrank */
			double tmpSimrank = calSimRank(qv, otherv);//, (qv == 0 && cur.getId() == 1));

			pq.add(new SimRankPair(otherv, tmpSimrank));
			if(pq.size() > k)
				pq.remove(); // remove the smallest one
		}
		
		/* output the answer */
		int cnt = k;
		System.out.println("SimRankMOD14 Method:");
		while(pq.isEmpty() == false) {
			SimRankPair srp = pq.poll();
			System.out.println(cnt+" ==> ("+ rvertexMap.get(srp.getVertex())+", " + String.format("%.9f", srp.getSimRank())+")");
			cnt--;
		}
	}

	private double calSimRank(int queryv, int otherv) {
		double simrank = 0;
		
		int[] queryPath = new int[sampleNum + 1];
		int[] otherPath = new int[sampleNum + 1];
		int[] commonEnds = new int[sampleNum + 1];
		int end = 0;
		
		for(int i = 0; i < sampleNum; i++){
			queryPath[i] = queryv;
			otherPath[i] = otherv;
		}
		HashSet<Integer> exist = new HashSet<Integer>();
		
		for(int i = 1; i <= MAX_STEPS; i++){
			end = 0;
			exist.clear();
			
			for(int x = 0; x < sampleNum; x++){
				if(queryPath[x] == -1 || exist.contains(queryPath[x])) continue;
				
				for(int y = 0; y < sampleNum; y++){
					if(queryPath[x] == otherPath[y]){
						commonEnds[end] = queryPath[x];
						exist.add(queryPath[x]);
						end++;
						break;
					}
				}
			}
			
			for(int z = 0; z < end; z++){
				int cntq = 0;
				int cnto = 0;
				for(int x = 0; x < sampleNum; x++){
					if(queryPath[x] == commonEnds[z]) cntq++;
					if(otherPath[x] == commonEnds[z]) cnto++;
				}
				simrank += 1.0 * cntq / sampleNum * cnto / sampleNum;
			}
			
			for(int x = 0; x < sampleNum; x++){
				ArrayList<Integer> nb;
				if(queryPath[x] != -1){
					nb = edgeList.get(queryPath[x]);
					if(nb == null || nb.size() == 0){
						queryPath[x] = -1;
					}
					else{
						queryPath[x] = nb.get((int)(getRandomNextInt() % nb.size()));
					}
				}
				
				if(otherPath[x] != -1){
					nb = edgeList.get(otherPath[x]);
					if(nb == null || nb.size() == 0){
						otherPath[x] = -1;
					}
					else{
						otherPath[x] = nb.get((int)(getRandomNextInt() % nb.size()));
					}
				}
			}
		}
		return simrank;
	}
	
	private long getRandomNextInt(){
		long rand = Random.nextInt();
		return rand < 0 ? -rand : rand;
	}
}
