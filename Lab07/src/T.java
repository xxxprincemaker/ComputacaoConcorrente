public class T extends Thread{
    //Identificador da Thread;
    private final int id;
    private final int[] vector;
    private final int numthreads;

    //Recurso Compartilhado
    S s;

    public T(int tid, S s, int v[], int nthread){
        this.id = tid;
        this.s = s;
        this.vector = v;
        this.numthreads = nthread;
    }

    public void run(){
        int bloco = (this.vector.length / this.numthreads);
        int inicio = this.id * bloco;

        int fim = 0;
        if(this.id == this.numthreads - 1) fim = this.vector.length;
        else fim = inicio + bloco;
        for (int i = inicio; i< fim; i++) {
            if((this.vector[i]%2) == 0) this.s.inc();
        }
    }
}
