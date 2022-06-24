class Leitor extends Thread {
    int id;
    Monitor monitor;
    Numero numero;

    Leitor (int id, Monitor m, Numero n) {
        this.id = id;
        this.monitor = m;
        this.numero = n;
    }

    public void run () {
        for (;;) {
            try {
                this.monitor.EntraLeitor(this.id);
                this.numero.imprimeParidade();
                this.monitor.SaiLeitor(this.id);
                sleep(1500);
            } catch (InterruptedException e) {
                return;
            }
        }
    }
}