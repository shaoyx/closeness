package com.graphtools.simrank;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.PriorityQueue;

import com.graphtools.utils.GraphAnalyticTool;
import com.graphtools.utils.MinHeap;
import com.graphtools.utils.Random;

@GraphAnalyticTool(
name = "SimRankWWW05",
description = "Sampling solution from WWW 2005 \"Scaling Link-Based Similarity Search\"."
)
public class SimRankWWW05 {

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
	
	private ArrayList<Fingerprint> index;
	
	public SimRankWWW05(SimRankUniverse sru){
		this.vertexMap = sru.vertexMap;
		this.rvertexMap = sru.rvertexMap;
		this.edgeList = sru.edgeList;
		this.sampleNum = sru.sampleNum;
		this.MAX_STEPS = sru.MAX_STEPS;
		this.qv = sru.qv;
		this.k = sru.k;
		this.decayFactor = sru.decayFactor;
		index = new ArrayList<Fingerprint>();
	}
	
	public void topksimrank() {
		PriorityQueue<SimRankPair> pq = new PriorityQueue<SimRankPair>();
		double[] simRankValue;
		
		for(int i = 0; i < sampleNum; i++){
			index.add(new Fingerprint(vertexMap.size()));
			indexBuilder(i);
		}
		
		simRankValue = calSimRank(qv);//, (qv == 0 && cur.getId() == 1));
		for(int otherv : vertexMap.values()){
			if(otherv == qv) continue;
			pq.add(new SimRankPair(otherv, simRankValue[otherv]));
			if(pq.size() > k)
				pq.remove(); // remove the smallest one
		}
		
		/* output the answer */
		int cnt = k;
		System.out.println("SimRankWWW05 Method:");
		while(pq.isEmpty() == false) {
			SimRankPair srp = pq.poll();
			System.out.println(cnt+" ==> ("+ rvertexMap.get(srp.getVertex())+", " + String.format("%.9f", srp.getSimRank())+")");
			cnt--;
		}
	}
	
	private void indexBuilder(int sn){
		int[] pathEnd = new int[vertexMap.size()];
		int[] nextIn = new int[vertexMap.size()];
		ArrayList<Integer> nb;
		
		for(int vid : vertexMap.values()){
			pathEnd[vid] = vid;
		}
		
		for(int s = 1; s <= MAX_STEPS; s++){
			for(int vid : vertexMap.values()){
				nb = edgeList.get(vid);
				if(nb == null || nb.size() == 0){
					nextIn[vid] = -1;
					continue;
				}
				nextIn[vid] = nb.get((int)(this.getRandomNextInt() % nb.size())); 
			}
			
			for(int vid : vertexMap.values()){
				if(pathEnd[vid] != -1){
					pathEnd[vid] = nextIn[pathEnd[vid]];
				}
			}
			
			/* detect the meet points */
			for(int vid : vertexMap.values()){
				nextIn[vid] = -1;
			}
			for(int vid : vertexMap.values()){
				if(pathEnd[vid] != -1){
					if(nextIn[pathEnd[vid]] == -1){
						nextIn[pathEnd[vid]] = vid; // save no edge
					}
					else if(nextIn[pathEnd[vid]] < vid){
						// save edge (vid, nextIn[pathEnd[vid]], s)
						index.get(sn).addEdge(vid, nextIn[pathEnd[vid]], s);
						pathEnd[vid] = -1;
					}
					else{
						// save edge (nextIn[pathEnd[vid]], vid,s);
						index.get(sn).addEdge(nextIn[pathEnd[vid]], vid, s);
						pathEnd[nextIn[pathEnd[vid]]] = -1;
						nextIn[pathEnd[vid]] = vid;
					}
				}
			}
		}
	}

	private double[] calSimRank(int queryv) {
		double[] simrank = new double[vertexMap.size() + 1];
		HashMap<Integer, Integer> queryPath;

		for(int otherv : vertexMap.values()){
			simrank[otherv] = 0.0;
		}
		
		for(int idx = 0; idx < sampleNum; idx++){
			if(index.get(idx).isExist(queryv)){
				queryPath = index.get(idx).getMeetPoints(queryv);
				for(int otherv : vertexMap.values()){
					if(otherv == queryv) continue;
					int meetTime = index.get(idx).calMeetTime(otherv, queryPath);
					if(meetTime > 0){
						simrank[otherv] += Math.pow(decayFactor, meetTime);
					}
				}
			}
		}
		
		for(int otherv : vertexMap.values()){
			simrank[otherv] /= sampleNum;
		}

	    return simrank;
	}
	
	private long getRandomNextInt(){
		long rand = Random.nextInt();
		return rand < 0 ? -rand : rand;
	}
}
