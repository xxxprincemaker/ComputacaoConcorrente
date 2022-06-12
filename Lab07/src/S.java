public class S {
    private int r;
    public S(){
        this.r = 0;
    }

    public synchronized void inc() {
        this.r++;
    }

    public  int get() {
        return this.r;
    }
}
