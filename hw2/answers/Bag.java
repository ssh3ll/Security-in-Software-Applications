
/* 
   ESC/Java2 Exercise:

   This class implements a Bag of integers, using an array.

   Add JML specs to this class, to stop ESC/Java2 from complaining.

   However, beware that there are also errors in the code that you may
   have to fix to stop ESC/Java2 from complaining. (More generally,
   feel free to improve the code to make it easier to specify in JML,
   but _only_ do this if you think this makes the code better/easier
   to understand.

   The only JML keywords needed for this are
      requires
      invariant
      ensures

   If you want, you can also use
      non_null
   as abbreviation.


   While developing your specs, it may be useful to use the keywords
      assert
   to add additional assertions in source code, to find out what
   ESC/Java2 can - or cannot - prove at a given program point.

*/

class Bag {

  int[] contents;	    //@ invariant contents != null;
  int n;              //@ invariant n >= 0;
                      //@ invariant n < contents.length;

  //@ requires input != null;
  //@ ensures contents.length == input.length;
  //@ ensures n == contents.length;
  public Bag(int[] input) {
    this.n = input.length;
    contents = new int[n];
    arraycopy(input, 0, contents, 0, n);
  }

  //@ ensures contents.length == 0;
  public Bag() {
    this.n =0;
    this.contents = new int[0];
  }

  //@ requires n > 0;
  //@ ensures n >= 0;
  void removeFirst(int elt) {
    for (int i = 0; i < n; i++) {  			// index must be less than or equals to n-1, not n
      if (contents[i] == elt ) {
    	  n--;
		contents[i] = contents[n];
		int[] t = new int[n];
		arraycopy(contents, 0, t, 0, n);
		contents = t;
		return;
      }
    }
  }

  //@ requires n > 0;
  public void removeAll(int elt) {
		boolean s = false;
		for (int i = 0; i < n; i++) {
			if (contents[i] == elt ) {
				s = true;
				 n--;
				 contents[i] = contents[n];
			}
		}

		// the following piece of code checks for remaining elt elements to remove.
		// without the following statements, if contents was like [155, 155, 160, 155]
		// and removeAll(155) is the call, this method returns, wrongly, [155,160].
		// It have to return [160] with contents=[155,155,160,155].
		if(s){
			while(s){
				s = false;
				for (int i = 0; i < n; i++) {
					if (contents[i] == elt ) {
						s = true;
						 n--;
						 contents[i] = contents[n];
					}
				}
			}
		}

		int[] t = new int[n];
		arraycopy(contents, 0, t, 0, n);
		contents = t;
	}

  int getCount(int elt) {
    int count = 0;
    for (int i = 0; i < n; i++)
      if (contents[i] == elt)
    	  count++;
    return count;
  }

  /* Warning: you may have a hard time checking the method "add" below.
     ESC/Java2 may warn about a very subtle bug that can be hard to spot.
     If you cannot find the problem after staring at the code for an hour,
     feel free to stop.
   */
	void add(int elt) {
		if (n == contents.length) {
			 int[] new_contents = new int[n+1];
			 arraycopy(contents, 0, new_contents, 0, n);
			 contents = new_contents;
		}
		contents[n]=elt;
		n++;
	}

  //@ requires b != null;
  //@ requires b.contents != null;
  void add(Bag b) {
    int[] new_contents = new int[n + b.n];
    arraycopy(contents, 0, new_contents, 0, n);
    arraycopy(b.contents, 0, new_contents, n, b.n);
    contents = new_contents;
  }

  //@ requires a != null;
  void add(int[] a) {
    this.add(new Bag(a));
  }

  //@ requires b != null;
  //@ requires b.contents != null;
  Bag(Bag b) {
	contents = new int[b.n];
	n = contents.length;
    this.add(b);
  }


  //@ requires src != null;
  //@ requires dest != null;
  //@ requires srcOff >= 0 && srcOff <= src.length;
  //@ requires destOff >= 0 && destOff <= dest.length;
  //@ requires length + srcOff <= src.length;
  //@ requires length + destOff <= dest.length;
  private static void arraycopy(int[] src,
                                int   srcOff,
                                int[] dest,
                                int   destOff,
                                int   length) {
    for( int i=0 ; i<length; i++) {
       dest[destOff+i] = src[srcOff+i];
    }
  }

}
