package com.graphtools.utils;

import java.util.ArrayList;

import com.graphtools.simrank.DegreeIdPair;

public class MinHeap {
	
	private int heapSize = 0;
	DegreeIdPair[] heapArray;
	int[] id2pos;
	
	public MinHeap() {}
	
	public void buildMinHeap(ArrayList<DegreeIdPair> al, int vsize){
		heapSize = al.size();
		heapArray = new DegreeIdPair[heapSize + 1];
		id2pos = new int[vsize + 1];
		for(int i = 1; i <= heapSize; i++){
			heapArray[i] = new DegreeIdPair(al.get(i-1).getDegree(), al.get(i-1).getId()); //copy the array
			id2pos[heapArray[i].getId()] = i;
		}
		build(); //build the heap with maintaining id2pos map.
	}
	
	public boolean isEmpty(){
		return heapSize == 0;
	}
	
	public DegreeIdPair pop(){
		int resPos = heapSize;
		swap(1, heapSize);
		heapSize--;
		minHeapifyDown(1);
		return heapArray[resPos];
	}
	
	public void update(int id){
		heapArray[id2pos[id]].decDegree(); // decrease one
		minHeapifyUp(id2pos[id]);
	}
	
	private void minHeapifyUp(int root){
		int child = root;
		int parent = root / 2;
		if(parent == 0 || heapArray[parent].getDegree() <= heapArray[child].getDegree()) 
			return;
		swap(parent, child);
		minHeapifyUp(parent);
	}
	

	private void build(){
		int pos = heapSize / 2;
		while(pos > 0){
			minHeapifyDown(pos);
			pos--;
		}
	}
	
	private void minHeapifyDown(int root){
		int min = root;
		int left  = root * 2;
		int right = root * 2 + 1;
		if(left <= heapSize && heapArray[min].getDegree() > heapArray[left].getDegree())
			min = left;
		if(right <= heapSize && heapArray[min].getDegree() > heapArray[right].getDegree())
			min = right;
		if(min != root){
			swap(min, root);
			minHeapifyDown(min);
		}
	}
	
	private void swap(int first, int second){
		int tmpdegree = heapArray[first].getDegree();
		int tmpid = heapArray[first].getId();
		heapArray[first].setDegree(heapArray[second].getDegree());
		heapArray[first].setId(heapArray[second].getId());
		id2pos[heapArray[second].getId()] = first; //record the id2pos here.
		
		heapArray[second].setDegree(tmpdegree);
		heapArray[second].setId(tmpid);
		id2pos[tmpid] = second; // record the id2pos here.
	}
	
	public String toString(){
		StringBuilder sb = new StringBuilder();
		sb.append("heap: ");
		for(int i = 1; i <= heapSize; i++){
			sb.append("(" + heapArray[i].getId()+", "+heapArray[i].getDegree()+") ");
		}
		sb.append("\nid2pos: ");
		for(int i = 0; i < id2pos.length; i++){
			sb.append("("+i+" ==> "+id2pos[i]+") "); 
		}
		return sb.toString();
	}
	
	public static void main(String[] args){
		ArrayList<DegreeIdPair> al = new ArrayList<DegreeIdPair>();
		int[] degree = {4,1,11,3,8,5,10,2,9,7};
		for(int i = 0; i < 10; i++){
			al.add(new DegreeIdPair(degree[i], i));
		}
		MinHeap mh = new MinHeap();
		mh.buildMinHeap(al, 10);
		System.out.println(mh.toString());
		System.out.println("update an element.");
		for(int i = 0; i < 5; i++){
			mh.update(5);
		}
		System.out.println(mh.toString());
		
		System.out.println("pop one element.");
		System.out.println(mh.pop());
		for(int i = 0; i < 5; i++){
			mh.update(9);
		}
		System.out.println(mh.toString());
		return ;
	}
}
