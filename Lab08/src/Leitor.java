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
//                System.out.println("O " + numero + " da Thread: " + this.id + " " + this.numero.isPar());
                System.out.println("le.paridade("+this.numero.getNumero()+","
                        +this.numero.isPar()+")");
                this.monitor.SaiLeitor(this.id);
                sleep(1500);
            } catch (InterruptedException e) {
                return;
            }
        }
    }
}