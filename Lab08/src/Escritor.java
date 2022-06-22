class Escritor extends Thread {
    int id;
    Monitor monitor;
    Numero numero;

    Escritor (int id, Monitor m, Numero n) {
        this.id = id;
        this.monitor = m;
        this.numero = n;
    }

    public void run () {
        for (;;) {
            try {
                this.monitor.EntraEscritor(this.id);
                numero.setNumero(numero.getNumero()+1);
                this.monitor.SaiEscritor(this.id);
                sleep(1500);
            } catch (InterruptedException e) {
                return;
            }
        }
    }
}