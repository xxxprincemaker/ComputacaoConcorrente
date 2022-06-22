class LeitorEscritor extends Thread {
    int id;
    Monitor monitor;
    Numero numero;

    LeitorEscritor (int id, Monitor m, Numero n) {
        this.id = id;
        this.monitor = m;
        this.numero = n;
    }

    public void run () {
        double j = 7777777.7, i;
        for (;;) {
            try {
                this.monitor.EntraLeitor(this.id);
                System.out.println("le.numero("+this.numero.getNumero()+")");
                this.monitor.SaiLeitor(this.id);
                for(i = 0; i < 10000000; i++) {j=j/2;} // Processamento bobo
                this.monitor.EntraEscritor(this.id);
                numero.setNumero(this.id);
                this.monitor.SaiEscritor(this.id);
                sleep(1500);
            } catch (InterruptedException e) {
                return;
            }
        }
    }
}