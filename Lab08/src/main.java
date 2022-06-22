public class main {

    static final int t2 = 3;
    static final int t1 = 2;
    static final int t3 = 5;

    public static void main (String[] args) {
        int i;
        Monitor monitor = new Monitor();
        Escritor[] e = new Escritor[t1];                //Thread 1
        Leitor[] l = new Leitor[t2];                    //Thread 2
        LeitorEscritor[] le = new LeitorEscritor[t3];   //Thread 3
        Numero numero = new Numero();

        System.out.println ("import verificaLE");
        System.out.println ("le = verificaLE.LE()");

        for (i=0; i< t1; i++) {
            e[i] = new Escritor(i+1, monitor, numero);
            e[i].start();
        }

        for (i=0; i< t2; i++) {
            l[i] = new Leitor(i+1, monitor, numero);
            l[i].start();
        }

        for (i=0; i< t3; i++) {
            le[i] = new LeitorEscritor(i+1, monitor, numero);
            le[i].start();
        }

    }

}
