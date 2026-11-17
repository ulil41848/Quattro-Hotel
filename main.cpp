#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdio>
#include <cctype>
#include <conio.h>
using namespace std;

// ===== Struct =====
struct Akun {
    string nama;
    string email;
    string username;
    string password;
};

struct Kamar {
    string nama;
    long long harga;
    string fasilitas;
};

struct Hotel {
    string nama;
    string alamat;
    string fasilitas;
    Kamar kamar[3];
};

struct ItemPesan {
    string nama;
    long long harga;
    int jumlah;
};

// Batas total kamar yang bisa dipesan dalam satu transaksi
const int BATAS_KAMAR = 100;

// Fasilitas kamar (dipakai semua hotel)
const string FAS_STD = "1 Kasur Queen, AC, TV, WiFi";
const string FAS_DLX = "1 Kasur King, AC, TV, Minibar, Wifi, Balkon";
const string FAS_SUI = "2 Kasur King, Ruang Tamu, Bathtub, Wifi, AC, View Kota";

// Format angka: 1500000 -> 1.500.000
string formatRibuan(long long value) {
    string angka = to_string(value);
    string hasil = "";
    int len = angka.length();
    for (int i = 0; i < len; i++) {
        hasil += angka[i];
        if ((len - i - 1) % 3 == 0 && i != len - 1) hasil += ".";
    }
    return hasil;
}

// Format harga: Rp. 1.500.000,00
string formatRupiah(long long value) {
    return "Rp. " + formatRibuan(value) + ",00";
}

// Baca satu baris lalu ubah jadi angka bulat.
// Jika bukan angka (huruf, kosong, simbol) -> kembalikan -1
int bacaAngka() {
    string s;
    getline(cin, s);
    if (s.empty() || s.length() > 9) return -1;
    for (int i = 0; i < (int)s.length(); i++) {
        if (!isdigit((unsigned char)s[i])) return -1;
    }
    return stoi(s);
}

// Validasi email harus @gmail.com
bool emailValid(string email) {
    string e = "";
    for (int i = 0; i < (int)email.length(); i++) e += tolower((unsigned char)email[i]);

    string akhiran = "@gmail.com";
    if (e.length() <= akhiran.length()) return false;
    if (e.substr(e.length() - akhiran.length()) != akhiran) return false;

    string depan = e.substr(0, e.length() - akhiran.length());
    for (int i = 0; i < (int)depan.length(); i++) {
        if (depan[i] == ' ' || depan[i] == '@') return false;
    }
    return true;
}

// Validasi username: wajib terisi dan hanya huruf
bool usernameValid(string u) {
    if (u.empty()) return false;
    for (int i = 0; i < (int)u.length(); i++) {
        if (!isalpha((unsigned char)u[i])) return false;
    }
    return true;
}

// Input password tampil bintang (*)
string inputPassword() {
    string pass = "";
    while (true) {
        char ch = getch();
        if (ch == 13) break;
        else if (ch == 8) {
            if (!pass.empty()) {
                pass.pop_back();
                cout << "\b \b";
            }
        }
        else if (ch >= 32 && ch <= 126) {
            pass += ch;
            cout << '*';
        }
    }
    cout << "\n";
    return pass;
}

// Input nominal bayar, titik pemisah muncul otomatis
long long inputNominal() {
    string angka = "";
    cout << "Masukkan nominal bayar : Rp. ";
    while (true) {
        char ch = getch();
        if (ch == 13) break;
        else if (ch == 8 && !angka.empty()) angka.pop_back();
        else if (ch >= '0' && ch <= '9' && angka.length() < 12) angka += ch;

        cout << "\rMasukkan nominal bayar : Rp.                    ";
        cout << "\rMasukkan nominal bayar : Rp. ";
        if (!angka.empty()) cout << formatRibuan(stoll(angka));
    }
    cout << "\n";
    if (angka.empty()) return 0;
    return stoll(angka);
}

// Tambah hari pada sebuah tanggal
tm tambahHari(tm dasar, int n) {
    dasar.tm_mday += n;
    dasar.tm_hour = 12;
    mktime(&dasar);
    return dasar;
}

// Format tanggal: "Sen, 10-12-2026"
string formatTanggal(tm t) {
    const char *hari[7] = {"Min", "Sen", "Sel", "Rab", "Kam", "Jum", "Sab"};
    char buf[30];
    snprintf(buf, sizeof(buf), "%s, %02d-%02d-%04d",
             hari[t.tm_wday], t.tm_mday, t.tm_mon + 1, t.tm_year + 1900);
    return string(buf);
}

int main() {
    string namaAplikasi = "QuattroHotel";
    string lokasi = "Kebumen, Jawa Tengah";

    Hotel daftarHotel[4] = {
        {"Hotel Mexolie",
         "Jl. Merak II, No 8 , Panjer",
         "WiFi, Restoran Seafood, Dekat Stasiun, Kolam Renang, Parkir",
         {{"Standard Room", 300000, FAS_STD},
          {"Deluxe Room",   450000, FAS_DLX},
          {"Suite Room",    800000, FAS_SUI}}},

        {"Hotel Grand Kolopaking",
         "Jl. Ahmad Yani, No 31, Kebumen",
         "WiFi, Restoran, Ruang Rapat, Parkir",
         {{"Standard Room", 350000, FAS_STD},
          {"Deluxe Room",   500000, FAS_DLX},
          {"Suite Room",    900000, FAS_SUI}}},

        {"Hotel Putra",
         "Jl. Pemuda, No 29, Kebumen",
         "WiFi, Lift, Parkir, Area Publik",
         {{"Standard Room", 400000,  FAS_STD},
          {"Deluxe Room",   600000,  FAS_DLX},
          {"Suite Room",    1000000, FAS_SUI}}},

        {"Hotel Trio Azana Style",
         "Jl. H.M Sarbini, No 1, Kebumen",
         "WiFi, Taman, Mall, Bioskop, Kolam Renang, Spa, Parkir",
         {{"Standard Room", 280000, FAS_STD},
          {"Deluxe Room",   420000, FAS_DLX},
          {"Suite Room",    750000, FAS_SUI}}}
    };

    Akun akun;

    // ===== #1 Pilihan Akun =====
    int pilihanAkun;
    cout << "=== SELAMAT DATANG DI APLIKASI " << namaAplikasi << " ===\n";
    cout << "1. Registrasi\n";
    cout << "2. Login\n";
    while (true) {
        cout << "Pilih: ";
        pilihanAkun = bacaAngka();
        if (pilihanAkun == 1 || pilihanAkun == 2) break;
        cout << "Silakan memilih antara 1 (Registrasi) atau 2 (Login)!\n";
    }

    // Pilih 2 (login) tapi belum punya akun -> arahkan ke registrasi
    if (pilihanAkun == 2) {
        cout << "\nAnda belum memiliki akun, silakan registrasi terlebih dahulu.\n";
    }

    // ===== Registrasi (untuk pilihan 1 maupun 2) =====
    cout << "\n--- REGISTRASI ---\n";

    do {
        cout << "Nama     : ";
        getline(cin, akun.nama);
        if (akun.nama.empty()) cout << "Nama wajib diisi!\n";
    } while (akun.nama.empty());

    do {
        cout << "Email    : ";
        getline(cin, akun.email);
        if (!emailValid(akun.email)) cout << "Email tidak valid, silakan coba lagi dengan format (nama@gmail.com)\n";
    } while (!emailValid(akun.email));

    do {
        cout << "Username : ";
        getline(cin, akun.username);
        if (!usernameValid(akun.username)) cout << "Username wajib diisi dan hanya boleh huruf!\n";
    } while (!usernameValid(akun.username));

    while (true) {
        cout << "Password (minimal 8 karakter) : ";
        akun.password = inputPassword();
        if (akun.password.empty()) cout << "Password wajib diisi!\n";
        else if (akun.password.length() < 8) cout << "Password minimal 8 karakter!\n";
        else break;
    }
    cout << "Registrasi berhasil!\n";

    // ===== Login =====
    string userLogin, passLogin;
    bool loginBerhasil = false;
    cout << "\n--- LOGIN ---\n";
    while (!loginBerhasil) {
        cout << "Username : ";
        getline(cin, userLogin);
        cout << "Password : ";
        passLogin = inputPassword();

        if (userLogin == akun.username && passLogin == akun.password) {
            loginBerhasil = true;
        } else {
            cout << "Username atau password salah, coba lagi.\n\n";
        }
    }

    // Ucapan selamat setelah login
    cout << "\n============================================\n";
    cout << "Selamat " << akun.nama << ", Anda telah login!\n";
    cout << "Selamat datang di aplikasi " << namaAplikasi << "\n";
    cout << "Lokasi hotel : " << lokasi << "\n";
    cout << "============================================\n";

    // ===== #2 Pilih Hotel =====
    cout << "\n=== DAFTAR HOTEL ===\n";
    for (int i = 0; i < 4; i++) {
        cout << i + 1 << ". " << daftarHotel[i].nama << "\n";
        cout << "   Alamat    : " << daftarHotel[i].alamat << "\n";
        cout << "   Fasilitas : " << daftarHotel[i].fasilitas << "\n";
    }

    int pilihanHotel;
    while (true) {
        cout << "\nPilih hotel (1-4): ";
        pilihanHotel = bacaAngka();
        if (pilihanHotel >= 1 && pilihanHotel <= 4) break;
        cout << "Silakan pilih hotel dari pilihan yang tersedia!\n";
    }
    Hotel hotelPilihan = daftarHotel[pilihanHotel - 1];
    cout << "Anda memilih " << hotelPilihan.nama << "\n";

    // ===== #3 Tanggal Check-in =====
    time_t sekarang = time(NULL);
    tm hariIni = *localtime(&sekarang);

    cout << "\n=== PILIH TANGGAL CHECK-IN ===\n";
    for (int i = 0; i < 7; i++) {
        cout << i + 1 << ". " << formatTanggal(tambahHari(hariIni, i)) << "\n";
    }

    int pilihIn;
    while (true) {
        cout << "Pilih tanggal check-in (1-7): ";
        pilihIn = bacaAngka();
        if (pilihIn >= 1 && pilihIn <= 7) break;
        cout << "Silakan pilih tanggal check-in dengan angka yang disediakan!\n";
    }
    tm tglIn = tambahHari(hariIni, pilihIn - 1);
    string tanggalCheckIn = formatTanggal(tglIn);

    // ===== #4 Tanggal Check-out =====
    cout << "\n=== PILIH TANGGAL CHECK-OUT ===\n";
    for (int i = 1; i <= 7; i++) {
        cout << i << ". " << formatTanggal(tambahHari(tglIn, i))
             << "  (" << i << " malam)\n";
    }

    int jumlahMalam;
    while (true) {
        cout << "Pilih tanggal check-out (1-7): ";
        jumlahMalam = bacaAngka();
        if (jumlahMalam >= 1 && jumlahMalam <= 7) break;
        cout << "Silakan pilih tanggal check-out dengan angka yang disediakan!\n";
    }
    tm tglOut = tambahHari(tglIn, jumlahMalam);
    string tanggalCheckOut = formatTanggal(tglOut);

    // ===== #5 & #6 Pilih Kamar =====
    ItemPesan pesanan[3];
    bool sudahDipilih[3] = {false, false, false};
    int jumlahItem = 0;
    int totalKamar = 0;     // total semua kamar yang sudah dipesan (maks 100)
    bool lagi = true;

    while (lagi) {
        int sisaKamar = BATAS_KAMAR - totalKamar;

        cout << "\n=== DAFTAR KAMAR - " << hotelPilihan.nama << " ===\n";
        cout << "(Sisa kuota kamar yang bisa dipesan: " << sisaKamar << ")\n";
        for (int i = 0; i < 3; i++) {
            cout << i + 1 << ". " << hotelPilihan.kamar[i].nama;
            if (sudahDipilih[i]) cout << "  (sudah dipilih)";
            cout << "\n";
            cout << "   Harga     : " << formatRupiah(hotelPilihan.kamar[i].harga) << " / malam\n";
            cout << "   Fasilitas : " << hotelPilihan.kamar[i].fasilitas << "\n";
        }

        // Pilih kategori kamar (tidak boleh kategori yang sudah dipesan)
        int pilihanKamar;
        while (true) {
            cout << "\nPilih kamar (1-3): ";
            pilihanKamar = bacaAngka();
            if (pilihanKamar < 1 || pilihanKamar > 3) {
                cout << "Silakan pilih kamar sesuai dengan angka yang disediakan!\n";
            } else if (sudahDipilih[pilihanKamar - 1]) {
                cout << "Anda sudah memesan kamar ini!\n";
            } else break;
        }

        // Jumlah kamar dibatasi oleh sisa kuota
        int jumlahKamar;
        while (true) {
            cout << "Jumlah kamar (1-" << sisaKamar << "): ";
            jumlahKamar = bacaAngka();
            if (jumlahKamar >= 1 && jumlahKamar <= sisaKamar) break;
            cout << "Silakan pilih jumlah kamar antara 1 sampai " << sisaKamar << "!\n";
        }

        pesanan[jumlahItem].nama = hotelPilihan.kamar[pilihanKamar - 1].nama;
        pesanan[jumlahItem].harga = hotelPilihan.kamar[pilihanKamar - 1].harga;
        pesanan[jumlahItem].jumlah = jumlahKamar;
        sudahDipilih[pilihanKamar - 1] = true;
        jumlahItem++;
        totalKamar += jumlahKamar;

        if (totalKamar >= BATAS_KAMAR) {
            // Kuota 100 kamar penuh -> langsung ke rincian harga tanpa y/n
            cout << "\nPesanan anda sudah memenuhi batas " << BATAS_KAMAR
                 << " kamar yang bisa dipesan.\n";
            lagi = false;
        } else if (jumlahItem >= 3) {
            // Ketiga kategori sudah dipilih -> otomatis ke rincian harga
            cout << "\nSemua kategori kamar sudah dipilih.\n";
            lagi = false;
        } else {
            // Masih ada kuota dan kategori lain -> tanya y/n
            while (true) {
                cout << "\nIngin memilih kategori kamar lain? (y/n): ";
                string jawab;
                getline(cin, jawab);
                if (jawab == "y" || jawab == "Y") { lagi = true; break; }
                if (jawab == "n" || jawab == "N") { lagi = false; break; }
                cout << "Silakan ketik y untuk memilih kamar lain atau n untuk lanjut!\n";
            }
        }
    }

    // ===== Hitung Total =====
    long long totalHarga = 0;
    cout << "\n--- RINCIAN HARGA ---\n";
    cout << "Hotel        : " << hotelPilihan.nama << "\n";
    cout << "Jumlah malam : " << jumlahMalam << "\n";
    for (int i = 0; i < jumlahItem; i++) {
        long long subtotal = pesanan[i].harga * pesanan[i].jumlah * jumlahMalam;
        totalHarga += subtotal;
        cout << "- " << pesanan[i].nama << " : " << formatRupiah(pesanan[i].harga)
             << " x " << pesanan[i].jumlah << " kamar x " << jumlahMalam << " malam = "
             << formatRupiah(subtotal) << "\n";
    }
    cout << "Total kamar  : " << totalKamar << "\n";
    cout << "Total harga  : " << formatRupiah(totalHarga) << "\n";

    // ===== #7 Metode Pembayaran =====
    string daftarBank[3] = {"BCA", "BRI", "Mandiri"};
    string daftarEwallet[3] = {"OVO", "GoPay", "Dana"};

    string metodeBayar = "";
    string statusPembayaran = "BELUM LUNAS";

    while (statusPembayaran != "LUNAS") {
        // Pilih jenis metode
        int metode;
        while (true) {
            cout << "\nMetode pembayaran [1. Transfer Bank | 2. E-Wallet]: ";
            metode = bacaAngka();
            if (metode == 1 || metode == 2) break;
            cout << "Silakan memilih metode pembayaran yang disediakan!\n";
        }

        // Pilih bank / e-wallet
        if (metode == 1) {
            cout << "\nPilih Bank:\n";
            for (int i = 0; i < 3; i++) cout << i + 1 << ". " << daftarBank[i] << "\n";
            int pilihBank;
            while (true) {
                cout << "Pilih bank (1-3): ";
                pilihBank = bacaAngka();
                if (pilihBank >= 1 && pilihBank <= 3) break;
                cout << "Silakan memilih metode pembayaran yang disediakan!\n";
            }
            metodeBayar = "Transfer Bank - " + daftarBank[pilihBank - 1];
        } else {
            cout << "\nPilih E-Wallet:\n";
            for (int i = 0; i < 3; i++) cout << i + 1 << ". " << daftarEwallet[i] << "\n";
            int pilihEwallet;
            while (true) {
                cout << "Pilih e-wallet (1-3): ";
                pilihEwallet = bacaAngka();
                if (pilihEwallet >= 1 && pilihEwallet <= 3) break;
                cout << "Silakan memilih metode pembayaran yang disediakan!\n";
            }
            metodeBayar = "E-Wallet - " + daftarEwallet[pilihEwallet - 1];
        }

        // ===== #8 Pembayaran (harus uang pas) =====
        cout << "\nSilakan melakukan pembayaran melalui " << metodeBayar << "\n";
        cout << "Total yang harus dibayar : " << formatRupiah(totalHarga) << "\n";
        cout << "(Harus dibayar dengan nominal pas)\n";

        long long nominalBayar = inputNominal();

        if (nominalBayar == totalHarga) {
            statusPembayaran = "LUNAS";
            cout << "Pembayaran berhasil!\n";
        } else {
            cout << "Nominal pembayaran tidak sesuai! Silakan ulangi.\n";
        }
    }

    // ===== #9 Konfirmasi Booking =====
    cout << "\n============================================\n";
    cout << "           KONFIRMASI BOOKING\n";
    cout << "============================================\n";
    cout << "Nama pelanggan  : " << akun.nama << "\n";
    cout << "Nama hotel      : " << hotelPilihan.nama << "\n";
    cout << "Lokasi          : " << hotelPilihan.alamat << "\n";
    cout << "Check-in        : " << tanggalCheckIn << "\n";
    cout << "Check-out       : " << tanggalCheckOut << "\n";
    cout << "Jumlah malam    : " << jumlahMalam << "\n";
    cout << "Kamar dipesan   :\n";
    for (int i = 0; i < jumlahItem; i++) {
        cout << "   - " << pesanan[i].nama << " x " << pesanan[i].jumlah << "\n";
    }
    cout << "Metode bayar    : " << metodeBayar << "\n";
    cout << "Total harga     : " << formatRupiah(totalHarga) << "\n";
    cout << "Status          : " << statusPembayaran << "\n";
    cout << "============================================\n";
    cout << "Yeayyy! Booking berhasil dilakukan!\n";

    // Simpan data pemesanan ke file
    ofstream file("booking.txt", ios::app);
    if (file.is_open()) {
        file << "Nama: " << akun.nama << " | Hotel: " << hotelPilihan.nama
             << " | " << tanggalCheckIn << " s/d " << tanggalCheckOut
             << " | Malam: " << jumlahMalam << " | Kamar: ";
        for (int i = 0; i < jumlahItem; i++) {
            file << pesanan[i].nama << " x" << pesanan[i].jumlah;
            if (i < jumlahItem - 1) file << ", ";
        }
        file << " | Bayar: " << metodeBayar
             << " | Total: " << formatRupiah(totalHarga)
             << " | " << statusPembayaran << "\n";
        file.close();
        cout << "Data pemesanan disimpan di 'booking.txt'\n";
    } else {
        cout << "Gagal menyimpan data pemesanan!\n";
    }

    cout << "\nTerima kasih telah melakukan pemesanan di " << namaAplikasi << "\n";
    return 0;
}
