typedef vecNum int64_t;
typedef angNum uint32_t;

class vec {
	vecNum x;
	vecNum y;
	vecNum distance(void);
	vec distance(vec base);
	vec normalize(vecNum distance);
	vec normalize(vecNum distance,vec base);
	ang vec::angle();
	vec operator+(const vec &b) const;
	vec operator+(vecNum b) const;
	vec operator-(const vec &b) const;
	vec operator-(vecNum b) const;
	vec operator*(const vec &b) const;
	vec operator*(vecNum b) const;
	vec operator-() const;
	vec operator/(const vec &b) const;
	vec operator/(vecNum b) const;
	vec operator+=(const vec &b) const;
	vec operator+=(vecNum b) const;
	vec operator-=(const vec &b) const;
	vec operator-=(vecNum b) const;
	vec operator*=(const vec &b) const;
	vec operator*=(vecNum b) const;
	vec operator/=(const vec &b) const;
	vec operator/=(vecNum b) const;
	vec operator==(const vec &b) const;
	vec operator==(vecNum b) const;
}

class ang {
	angNum n;
}