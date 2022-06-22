import java.lang.Thread;
import java.util.Random;

public class Main {
    static final int N = 12; //Esse é o numero de nucleos do meu computador.
    static final int tamVetor = 100000;
    static int pares = 0;
    public static void foo(int[] v){
        for (int j : v) {
            if ((j % 2) == 0) pares++;
        }
    }
    public static void main(String[] args) {
        //reserva espaço para um vetor de threads
        Thread[] threads = new Thread[N];
        Random random = new Random();
        int[] v = new int[tamVetor];

        //cria uma instancia do recurso compartilhado entre as threads
        S s = new S();

        //Inicializando Vetor
        for (int i = 0; i < v.length; i++){
            v[i] =  random.nextInt(tamVetor);
        }

        long startSq = System.currentTimeMillis();
        foo(v);
        long elapsedSq = System.currentTimeMillis() - startSq;
        System.out.print("Tempo decorrido sequencial (segundos): " + (elapsedSq) + "\n");
        System.out.println("Valor de Numeros Pares Sequencial = " + pares);

        long start = System.currentTimeMillis();
        //cria as threads da aplicacao
        for (int i = 0; i < threads.length; i++) {
            threads[i] = new T(i, s, v, N);
        }

        //inicia as threads
        for (int i = 0; i < threads.length; i++) {
            threads[i].start();
        }

        //espera pelo termino de todas as threads
        for (int i = 0; i < threads.length; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                return;
            }
        }
        long elapsed = System.currentTimeMillis() - start;
        System.out.print("Tempo decorrido concorrente (segundos): " + elapsed + "\n");

        System.out.println("Valor de Numeros Pares Concorrente = " + s.get());
    }
}
