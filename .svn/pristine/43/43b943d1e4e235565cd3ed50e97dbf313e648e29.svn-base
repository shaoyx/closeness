package com.graphtools.simrank;

import java.util.HashMap;

/**
 * This class will cost huge memory.
 * @author simon0227
 *
 */
public class PositionProbability {
	private HashMap<Integer, HashMap<Integer, Double>> probList;
	
	public PositionProbability(){
		probList = new HashMap<Integer, HashMap<Integer, Double>>();
	}
	
	public void add(int step, int vid, double prob){
		HashMap<Integer, Double> specPosProb = probList.get(step);
		if(specPosProb == null){
			specPosProb = new HashMap<Integer, Double>();
			probList.put(step, specPosProb);
		}
		Double cur = specPosProb.get(vid);
		if(cur == null)
			specPosProb.put(vid, prob);
		else 
			specPosProb.put(vid, prob + cur);
	}
	
	public double getProb(int step, int vid){
		HashMap<Integer, Double> specPosProb = probList.get(step);
		if(specPosProb == null)
			return 0.0;
		return specPosProb.get(vid);
	}
	
	public HashMap<Integer, Double> getProbAtaTime(int step){
		return probList.get(step);
	}
	
	public String toString(){
		String ans = "";
		for(int i = 1; i <= 10; i++){
			if(probList.get(i) == null)
				 break;
			double accu = 0;
			ans += "step="+i+": ";
			for(int vid : probList.get(i).keySet()){
				ans += "("+vid+", "+String.format("%.3f", probList.get(i).get(vid))+") ";
				accu += probList.get(i).get(vid);
			}
			ans+="total="+String.format("%.3f", accu)+'\n';
		}
		return ans;
	}
	
}
