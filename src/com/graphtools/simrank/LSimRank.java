package com.graphtools.simrank;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.PriorityQueue;

import com.graphtools.utils.GraphAnalyticTool;
import com.graphtools.utils.MinHeap;

/**
 * Local SimRank computation using memorization.
 * @author simon0227
 *
 */
@GraphAnalyticTool(
name = "LSimRank",
description = "local simrank computation which relies on RandomWalks."
)
/**
 * This solution computes simrank based on random walks with non-first-meeting guarantee.
 * This solution doesn't rely on the sampling method. It enumerates all the random walks.
 * 
 * @author ggbond
 *
 */
public class LSimRank {

	private MinHeap mh = new MinHeap(); //a scheduler
	private SimRankUniverse sru;
	
	public LSimRank(SimRankUniverse sru){
		this.sru = sru;
		ArrayList<DegreeIdPair> al = new ArrayList<DegreeIdPair>();
		for(int vid : sru.edgeList.keySet()){
			al.add(new DegreeIdPair(sru.edgeList.get(vid).size(), vid));
		}
		mh.buildMinHeap(al, sru.vertexMap.size());
	}
	
	/**
	 * 1. calculate query vertex's probability
	 * 2. enumerate each other vertex
	 * 3. get the top-k SimRank
	 *  
	 * @param vertex
	 * @param k
	 */
	public void topkcomputation() {
		int qv = sru.qv;
		int k = sru.k;
		
		PositionProbability queryVertexPP = computeVertexProb(qv);
		PriorityQueue<SimRankPair> pq = new PriorityQueue<SimRankPair>();
		
		/* TODO: here enumerate all the vertices. */
		while(mh.isEmpty() == false){
			DegreeIdPair cur = mh.pop();
			if(cur.getId() == qv) continue; // do not recompute qv.
			/* TODO: costy here! */
			PositionProbability curPP = computeVertexProb(cur.getId());
			/* calculate simrank */
//			if(qv == 0 && cur.getId() == 1){
//				System.out.println("queryVertex "+ qv +":\n"+queryVertexPP.toString());
//				System.out.println("partVertex "+ cur.getId() +":\n"+curPP.toString());
//			}
			double tmpSimrank = calSimRank(queryVertexPP, curPP);//, (qv == 0 && cur.getId() == 1));
//			if(qv == 0 && cur.getId() == 1){
//				System.out.println("simrank = "+ String.format("%.9f", tmpSimrank));
//			}
			pq.add(new SimRankPair(cur.getId(), tmpSimrank));
			if(pq.size() > k)
				pq.remove(); // remove the smallest one
		}
		/* output the answer */
		int cnt = k;
		System.out.println("LSimRank Method:");
		while(pq.isEmpty() == false){
			SimRankPair srp = pq.poll();
			System.out.println(cnt+" ==> ("+ sru.rvertexMap.get(srp.getVertex())+", " + String.format("%.9f", srp.getSimRank())+")");
			cnt--;
		}
	}

	private double calSimRank(PositionProbability queryVertexPP,
			PositionProbability curPP){//, boolean isPrint) {
		double simrank = 0;
		double decayFactor = sru.decayFactor; 
		double factor = 1;
		for(int i = 1; i <= sru.MAX_STEPS; i++){
			HashMap<Integer, Double> prob1 = queryVertexPP.getProbAtaTime(i);
			HashMap<Integer, Double> prob2 = curPP.getProbAtaTime(i);
			factor *= decayFactor;
			double tmpSimrank = 0;
			if(prob1 == null || prob2 == null) continue;
			for(int vid : prob1.keySet()){
				Double tmp = prob2.get(vid);
				if(tmp == null) continue;
				 tmpSimrank += factor * tmp * prob1.get(vid); // no first-meeting guarantee.
			}
//			if(isPrint){
//				System.out.println("step="+i+": tmpsimrank = "+ String.format("%.9f", tmpSimrank));
//			}
			simrank += tmpSimrank;
		}
		return simrank;
	}

	/**
	 * random walk trees. Compute them iteratively. 
	 * The vertices are allowed to revisited.
	 * @param vertex
	 * @return
	 */
	private PositionProbability computeVertexProb(Integer vertex) {
		PositionProbability res = new PositionProbability();
		HashMap<Integer, Double> extendCand = new HashMap<Integer, Double>();
		extendCand.put(vertex, 1.0);
		for(int i = 1; i <= sru.MAX_STEPS; i++){
			if(extendCand == null){
				extendCand = res.getProbAtaTime(i);
				continue;
			}
			for(int vid : extendCand.keySet()){
				ArrayList<Integer> nb = sru.edgeList.get(vid);
				if(nb == null) continue;
				double prob = extendCand.get(vid) / nb.size();
				for(int nbvid : nb){
					int nextId = nbvid;
					res.add(i, nextId, prob);
				}
			}
			extendCand = res.getProbAtaTime(i);
		}
		return res;
	}
}
