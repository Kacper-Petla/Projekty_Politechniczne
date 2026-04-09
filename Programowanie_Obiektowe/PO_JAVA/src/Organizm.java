
public abstract class Organizm
{
    protected int sila;
    protected int inicjatywa;
    protected int x, y;
    protected Swiat swiat;
    protected int wiek;
    protected Stale.TypOrganizmu typ;

    protected int gx, gy;
    protected int tmp_sila;
    protected int umiejetnosc;

    public Organizm(int sila, int inicjatywa, int x, int y, Swiat swiat, Stale.TypOrganizmu typ)
    {
        this.sila = sila;
        this.inicjatywa = inicjatywa;
        this.x = x;
        this.y = y;
        this.swiat = swiat;
        this.wiek = 0;
        this.typ = typ;
    }

    public int getX() { return x; }
    public int getY() { return y; }
    public void setX(int newX) { this.x = newX; }
    public void setY(int newY) { this.y = newY; }

    public int getSila() { return sila; }
    public int getInicjatywa() { return inicjatywa; }
    public void setSila(int newSila) { this.sila = newSila; }

    public int getWiek() { return wiek; }
    public void setWiek(int newWiek) { this.wiek = newWiek; }
    public Stale.TypOrganizmu getTyp() { return typ; }

    public void setGX(int newGX) { this.gx = newGX; }
    public void setGY(int newGY) { this.gy = newGY; }

    public int getTmp_Sila() { return tmp_sila; }
    public void setTmp_Sila(int newTmp_Sila) { this.tmp_sila = newTmp_Sila; }

    public int getUmiejetnosc() { return umiejetnosc; }
    public void setUmiejetnosc(int newUmiejetnosc) { this.umiejetnosc = newUmiejetnosc; }

    public abstract void akcja();
    public abstract void kolizja(Organizm inny);
    public abstract void rozmnazanie();
    public abstract char rysowanie();
    public abstract String nazwa();
    public abstract Organizm stworzPotomka(int x, int y);
}