#ifndef __VALUEPAIR_H__
#define __VALUEPAIR_H__

class ValuePair{
	public:
		ValuePair() : vid(-1), value(0.0) {}
		ValuePair(int id, double v) : vid(id), value(v){}

		int getVid() const { return vid; }
		double getValue() const { return value; }

		void setVid(int v) { vid = v; }
		void setValue(double val) { value = val; }

		bool operator< (const ValuePair& other) const{
			return value > other.value;
		}

	private:
		int vid;
		double value;
};
#endif
