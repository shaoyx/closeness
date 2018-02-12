package com.graphtools.simrank;

import java.util.HashMap;
import java.util.HashSet;

public class Fingerprint {
	private HashSet<Integer> vset;
	private int[] edge;
	private int[] weights;
	private int vnum;
	
	public Fingerprint(int vnum){
		this.vnum = vnum;
		edge = new int[vnum + 1];
		weights = new int[vnum + 1];
		for(int i = 0; i <= vnum; i++){
			edge[i] = -1;
		}
		vset = new HashSet<Integer>();
	}
	
	public void addEdge(int src, int dst, int w){
		edge[src] = dst;
		weights[src] = w;
		vset.add(src);
		vset.add(dst);
	}
	
	public boolean isExist(int v){
		return vset.contains(v);
	}

	public HashMap<Integer, Integer> getMeetPoints(int qv) {
		HashMap<Integer, Integer> result = new HashMap<Integer, Integer>();
		result.put(qv, 0);
		int cur = qv;
		while(edge[cur] != -1){
			result.put(edge[cur], weights[cur]);
			cur = edge[cur];
		}
		return result;
	}

	public int calMeetTime(int otherv, HashMap<Integer, Integer> queryPath) {
		int cur = otherv;
		int w = 0;
		int res = 0;
		do{
			if(queryPath.get(cur) != null){
				res = Math.max(w, queryPath.get(cur));
				break;
			}
			w = weights[cur];
			cur = edge[cur];
		}while(cur != -1);
		return res;
	}

}
